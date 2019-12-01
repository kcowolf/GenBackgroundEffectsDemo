#include "BATL.h"
#include "DAT.h"
#include "GFX.h"

const DAT_BattleBackgroundType DAT_battle_backgrounds[DAT_BACKGROUND_COUNT] =
{
    {
        (const u32*)GFX_TILESET_BKG1,
        GFX_TILESET_BKG1_TILE_COUNT,
        GFX_TILEMAP_BKG1,
        GFX_PAL_BKG1,
        1,
        0,
        BATL_PALETTE_CYCLE_ENABLED,
        2,
        6,
        16
    },
    {
        (const u32*)GFX_TILESET_BKG1,
        GFX_TILESET_BKG1_TILE_COUNT,
        GFX_TILEMAP_BKG2,
        GFX_PAL_BKG1,
        2,
        2,
        BATL_PALETTE_CYCLE_NONE,
        2,
        6,
        16
    },
    {
        (const u32*)GFX_TILESET_BKG1,
        GFX_TILESET_BKG1_TILE_COUNT,
        GFX_TILEMAP_BKG1,
        GFX_PAL_BKG1,
        3,
        1,
        BATL_PALETTE_CYCLE_ENABLED,
        2,
        6,
        16
    }
};

const DAT_BattleBackgroundDistortionType DAT_battle_background_distortions[] =
{
    {
        BATL_OSCILLATION_HORIZONTAL,
        0,
        0,
        0
    },
    {
        BATL_OSCILLATION_HORIZONTAL,
        3,
        24,
        16
    },
    {
        BATL_OSCILLATION_HORIZONTAL_INTERLEAVED,
        5,
        4,
        8
    },
    {
        BATL_OSCILLATION_VERTICAL,
        3,
        24,
        16
    }
};

const DAT_BattleBackgroundScrollingType DAT_battle_background_scrolls[] =
{
    {
        0,
        0
    },
    {
        1,
        0
    },
    {
        0,
        1
    }
};
