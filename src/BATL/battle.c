#include "BATL.h"
#include "UTIL_256x224.h"

#include "background.h"

void BATL_init
    (
    u16 background_idx
    )
{
    // Initialize the video processor, set screen resolution to 256x224
    VDP_init();
    VDP_setScreenWidth256();
    VDP_setScreenHeight224();

    // Set up VDP modes
    VDP_setHilightShadow(0);

    // Set up scroll planes
    VDP_setPlanSize(UTIL_VDP_PLANE_TILE_WIDTH, UTIL_VDP_PLANE_TILE_HEIGHT);

    background_init(background_idx);
}

void BATL_update()
{
    background_update();
}

void BATL_update_VDP()
{
    background_update_VDP();
}

void BATL_teardown()
{
    background_teardown();
}
