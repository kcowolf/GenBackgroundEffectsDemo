#ifndef BATL_H
#define BATL_H

#include <genesis.h>

typedef enum
{
    BATL_OSCILLATION_HORIZONTAL,
    BATL_OSCILLATION_HORIZONTAL_INTERLEAVED,
    BATL_OSCILLATION_VERTICAL
} BATL_OscillationType;

typedef enum
{
    BATL_PALETTE_CYCLE_NONE,
    BATL_PALETTE_CYCLE_ENABLED,
} BATL_PaletteCycleType;

void BATL_init
    (
    u16 background_idx
    );

void BATL_update();
void BATL_update_VDP();
void BATL_teardown();

#endif // BATL_H
