#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include "raylib.h"
#include "./defs.h"

// Global variables

extern int global_paused; // Game pause state

extern float global_cam_yaw;  // left/right
extern float global_cam_pitch; // up/down

void pause_toggle(); // Toggle pause state

#endif // GLOBAL_H