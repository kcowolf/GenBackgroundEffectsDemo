// Originally based on explanation found here: https://forum.starmen.net/forum/General/Tech/Help-with-allegro-and-c/page/1#post1131426

#include <genesis.h>
#include "BATL.h"
#include "CAM.h"
#include "DAT.h"
#include "GFX.h"
#include "UTIL.h"
#include "UTIL_256x224.h"

#include "background.h"

#define PAL PAL0
#define PLANE_ADDR VDP_PLAN_B
#define PLANE PLAN_B
#define TILE_START_IDX 1

// Backgrounds must be 256x256.
#define BACKGROUND_WIDTH_TILE 32
#define BACKGROUND_HEIGHT_TILE 32

// SGDK Map structure; w and h are width and height in tiles.
static Map sgdkMap;

static u16 frame_counter;
static volatile u16 scanline_counter;

static s16 offsets[UTIL_SCREEN_PIXEL_HEIGHT];

static u16 palette_cycle_delay_frames_left;
static u16 palette[UTIL_PALETTE_COLOR_COUNT];

static u16 initial_sine_table_idx;

static const DAT_BattleBackgroundType* background_data;
static const DAT_BattleBackgroundDistortionType* distortion_data;
static const DAT_BattleBackgroundScrollingType* scrolling_data;

static void hint();
static void redraw_background();

void background_init
    (
    u16 background_idx
    )
{
    background_data = &DAT_battle_backgrounds[background_idx];
    distortion_data = &DAT_battle_background_distortions[background_data->distortion_type];
    scrolling_data = &DAT_battle_background_scrolls[background_data->scrolling_type];

    sgdkMap.w = BACKGROUND_WIDTH_TILE;
    sgdkMap.h = BACKGROUND_HEIGHT_TILE;
    sgdkMap.tilemap = (u16*)background_data->tilemap;

    // Load palette
    palette_cycle_delay_frames_left = (background_data->palette_cycle_mode == BATL_PALETTE_CYCLE_NONE) ? 1 : background_data->palette_cycle_delay_frames;

    memcpy((void*)palette, background_data->palette, UTIL_PALETTE_COLOR_COUNT * sizeof(u16));
    VDP_setPalette(PAL, palette);

    // Load tiles
    VDP_loadTileData(background_data->tiles, TILE_START_IDX, background_data->tile_count, 0);

    // Initialize camera
    CAM_init(0, 0, 0, 0, -1, -1, UTIL_SCREEN_PIXEL_WIDTH, UTIL_SCREEN_PIXEL_HEIGHT);
    CAM_update();

    BATL_update_VDP();
    redraw_background();

    frame_counter = 0;
    scanline_counter = 0;
    memset(offsets, 0, UTIL_SCREEN_PIXEL_HEIGHT * sizeof(s16));

    initial_sine_table_idx = distortion_data->frequency * UTIL_SCREEN_PIXEL_HEIGHT_MINUS_ONE + distortion_data->time_shifting * frame_counter;

    if (distortion_data->oscillation_mode == BATL_OSCILLATION_VERTICAL)
    {
        VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

        // Enable horizontal interrupts
        SYS_setHIntCallback(hint);
        VDP_setHIntCounter(1); // Every 2 lines
        VDP_setHInterrupt(TRUE);
    }
    else
    {
        VDP_setScrollingMode(HSCROLL_LINE, VSCROLL_PLANE);
    }
}

void background_update()
{
    CAM_px += scrolling_data->horizontal_velocity;
    CAM_py += scrolling_data->vertical_velocity;
    
    u16 sine_table_idx = initial_sine_table_idx;    
    initial_sine_table_idx += distortion_data->time_shifting;

    u16 i = UTIL_SCREEN_PIXEL_HEIGHT;

    do
    {
        i--;

        s16 offset = fix16ToInt(sinFix16(sine_table_idx) << distortion_data->amplitude);
        if (distortion_data->oscillation_mode == BATL_OSCILLATION_HORIZONTAL)
        {
            offsets[i] = -(CAM_px + offset);
        }
        else if (distortion_data->oscillation_mode == BATL_OSCILLATION_HORIZONTAL_INTERLEAVED)
        {
            offsets[i] = (i & 1) ? -(CAM_px + offset) : -(CAM_px - offset);
        }
        else
        {
            offsets[i] = CAM_py + offset;
        }

        sine_table_idx -= distortion_data->frequency;
    } while (i != 0);

    if (distortion_data->oscillation_mode != BATL_OSCILLATION_VERTICAL)
    {
        // Queue new line offsets for DMA transfer during next VBLANK.
        VDP_setHorizontalScrollLine(PLANE, 0, offsets, UTIL_SCREEN_PIXEL_HEIGHT, DMA_QUEUE);
    }

    if (background_data->palette_cycle_mode == BATL_PALETTE_CYCLE_ENABLED)
    {
        palette_cycle_delay_frames_left--;
        if (palette_cycle_delay_frames_left == 0)
        {
            u16 i;
            u16 temp_color;
            
            temp_color = palette[background_data->palette_cycle_max_idx];
            for (i = background_data->palette_cycle_max_idx; i != background_data->palette_cycle_min_idx; i--)
            {
                palette[i] = palette[i - 1];
            }
            palette[background_data->palette_cycle_min_idx] = temp_color;
        }
    }
}

void background_update_VDP()
{
    scanline_counter = 0;
    frame_counter++;

    // Foreground
    if (distortion_data->oscillation_mode == BATL_OSCILLATION_VERTICAL)
    {
        VDP_setHorizontalScroll(PLAN_B, -CAM_px);
        VDP_setVerticalScroll(PLAN_B, offsets[0]);
    }
    else
    {
        VDP_setHorizontalScroll(PLAN_B, -(offsets[0]));
        VDP_setVerticalScroll(PLAN_B, CAM_py);
    }

    if (palette_cycle_delay_frames_left == 0)
    {
        VDP_setPalette(PAL, palette);
        palette_cycle_delay_frames_left = background_data->palette_cycle_delay_frames;
    }
}

void background_teardown()
{
    // Disable horizontal interrupts
    VDP_setHInterrupt(FALSE);
    SYS_setHIntCallback(NULL);
}

// Redraw the whole screen.  Normally this would be done with the screen blacked out.
static void redraw_background()
{
    u16 col_buffer[UTIL_VDP_PLANE_TILE_HEIGHT];

    u16 current_col = UTIL_SCREEN_TILE_WIDTH_PLUS_ONE;
    do
    {
        current_col--;

        // Calculate where in the tilemap the new row's tiles are located.
        const u16* mapDataAddr = sgdkMap.tilemap + current_col;

        u16 columnBufferIdx = 0;
        u16 baseTile = TILE_ATTR_FULL(PAL, 0, 0, 0, TILE_START_IDX);

        // Copy the tiles into the buffer.
        u16 i;
        for (i = UTIL_VDP_PLANE_TILE_HEIGHT; i != 0; i--)
        {
            columnBufferIdx &= 0x1F;  // columnBufferIdx MOD 32 (VDP_PLANE_TILE_HEIGHT)
            col_buffer[columnBufferIdx] = baseTile + *mapDataAddr;
            columnBufferIdx++;
            mapDataAddr += sgdkMap.w;
        }

        // Since we're redrawing the whole screen, do the DMA immediately instead of queuing it up.
        DMA_doDma(DMA_VRAM, (u32) col_buffer, PLANE_ADDR + (((current_col) & UTIL_VDP_PLANE_TILE_WIDTH_MINUS_ONE) << 1), UTIL_VDP_PLANE_TILE_HEIGHT, UTIL_VDP_PLANE_TILE_WIDTH_TIMES_TWO);
    }
    while (current_col != 0);
}

static void hint()
{
    scanline_counter++;
    VDP_setVerticalScroll(PLAN_B, offsets[scanline_counter]);
}
