#ifndef DAT_H
#define DAT_H

#include <genesis.h>

#define DAT_BACKGROUND_COUNT 3

typedef struct
{
    const u32* tiles;
    const u16 tile_count;
    const u16* tilemap;
    const void* palette;
    u8 distortion_type;
    u8 scrolling_type;
    u8 palette_cycle_mode;
    u8 palette_cycle_min_idx;
    u8 palette_cycle_max_idx;
    u8 palette_cycle_delay_frames;
} DAT_BattleBackgroundType;

typedef struct
{
    u8 oscillation_mode;
    u16 amplitude;
    u16 frequency;
    u16 time_shifting;
} DAT_BattleBackgroundDistortionType;

typedef struct
{
    s8 horizontal_velocity;
    s8 vertical_velocity;
} DAT_BattleBackgroundScrollingType;

extern const DAT_BattleBackgroundType DAT_battle_backgrounds[DAT_BACKGROUND_COUNT];
extern const DAT_BattleBackgroundDistortionType DAT_battle_background_distortions[];
extern const DAT_BattleBackgroundScrollingType DAT_battle_background_scrolls[];

#endif // DAT_H
