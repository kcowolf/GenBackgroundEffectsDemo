#ifndef CAM_H
#define CAM_H

#include <genesis.h>

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
    );

void CAM_update();

// Coordinates of the camera (top-left pixel on the screen).  These will be clamped to be within the map boundaries (CAM_px_min,CAM_py_min)-(CAM_px_max,CAM_py_max).
extern u32 CAM_px;
extern u32 CAM_py;

// Minimum coordinates (towards the top left) where the camera can be without showing anything beyond the room edges.
extern u32 CAM_px_min;
extern u32 CAM_py_min;

// Maximum coordinates (towards the bottom right) where the camera can be without showing anything beyond the room edges.
extern u32 CAM_px_max;
extern u32 CAM_py_max;

#endif // CAM_H
