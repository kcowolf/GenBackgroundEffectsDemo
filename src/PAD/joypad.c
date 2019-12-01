#include "PAD.h"

u16 PAD_state;

void PAD_init()
{
    // Initialize the controllers and set up the controller callback function
    JOY_init();
    JOY_setEventHandler(NULL);
}

void PAD_update()
{
    PAD_state = JOY_readJoypad(JOY_1);  // SGDK call
}
