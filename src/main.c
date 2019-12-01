#include <genesis.h>
#include "BATL.h"
#include "DAT.h"
#include "CAM.h"
#include "GFX.h"
#include "PAD.h"

void set_background(u16 background_idx)
{
    BATL_teardown();
    BATL_init(background_idx);
}

int main()
{
    u16 background_idx = 0;
    u16 background_change_delay = 0;

    BATL_init(background_idx);
    PAD_init();

    while(1)
    {
        // Control pads
        PAD_update();

        if (background_change_delay == 0)
        {
            if (PAD_state & BUTTON_UP)
            {
                background_idx++;
                if (background_idx == DAT_BACKGROUND_COUNT)
                {
                    background_idx = 0;
                }
                
                background_change_delay = 20;
                set_background(background_idx);
            }
            else if (PAD_state & BUTTON_DOWN)
            {
                if (background_idx == 0)
                {
                    background_idx = DAT_BACKGROUND_COUNT;
                }
                background_idx--;

                background_change_delay = 20;
                set_background(background_idx);
            }
        }
        else
        {
            background_change_delay--;
        }

        // Camera
        CAM_update();

        // Map
        BATL_update();

        // Wait for vsync
        VDP_waitVSync();

        // Map
        BATL_update_VDP();
    }
}
