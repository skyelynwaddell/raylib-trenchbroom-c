#ifndef GLOBAL_H
#define GLOBAL_H

#include "defs.h"
#include "raylib.h"

// Global variables

extern int global_paused; // Game pause state

extern Vector3 global_player_spawn;
extern int global_player_onground;

void pause_toggle(); // Toggle pause state

#endif // GLOBAL_H