#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include "raylib.h"
#include "./defs.h"
#include "event.h"

// Global variables

extern int global_paused; // Game pause state

extern float global_cam_yaw;  // left/right
extern float global_cam_pitch; // up/down

extern Vector3 global_player_spawn;
extern int global_player_onground;

extern Event events[255];

void pause_toggle(); // Toggle pause state
float to_delta(float value);

#endif // GLOBAL_H