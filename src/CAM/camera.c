#include "CAM.h"

u32 CAM_px;
u32 CAM_py;

u32 CAM_px_min;
u32 CAM_py_min;
u32 CAM_px_max;
u32 CAM_py_max;

void CAM_init
    (
    u32 px,
    u32 py,
    u32 px_min,
    u32 py_min,
    u32 px_max,
    u32 py_max,
    u16 screen_pw,
    u16 screen_ph
    )
{
    CAM_px = px;
    CAM_py = py;
    CAM_px_min = px_min;
    CAM_py_min = py_min;

    if (px_max < screen_pw || px_max - screen_pw < CAM_px_min)
    {
        // Room is too narrow to scroll horizontally
        CAM_px_max = CAM_px_min;
    }
    else
    {
        CAM_px_max = px_max - screen_pw;
    }

    if (py_max < screen_ph || py_max - screen_ph < CAM_py_min)
    {
        // Room is too narrow to scroll vertically
        CAM_py_max = CAM_py_min;
    }
    else
    {
        CAM_py_max = py_max - screen_ph;
    }
}

void CAM_update()
{
    if (CAM_px < CAM_px_min)
    {
        CAM_px = CAM_px_min;
    }
    else if (CAM_px > CAM_px_max)
    {
        CAM_px = CAM_px_max;
    }

    if (CAM_py < CAM_py_min)
    {
        CAM_py = CAM_py_min;
    }
    else if (CAM_py > CAM_py_max)
    {
        CAM_py = CAM_py_max;
    }
}
