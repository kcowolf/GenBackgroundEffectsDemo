#ifndef BATTLE_BACKGROUND_H
#define BATTLE_BACKGROUND_H

#include <genesis.h>

void background_init
    (
    u16 background_idx
    );

void background_update();

void background_update_VDP();

void background_teardown();

#endif // BATTLE_BACKGROUND_H
