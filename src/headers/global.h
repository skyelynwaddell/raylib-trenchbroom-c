#ifndef GLOBAL_H
#define GLOBAL_H

#include "defs.h"
#include "raylib.h"
#include "anims.h"

// Global variables

extern int global_paused; // Game pause state
extern int global_raycast_has_target;

extern Vector3 global_player_spawn;
extern int global_player_onground;
extern int global_player_crouching;
extern int global_player_shooting;
extern const char global_player_name[64];

void pause_toggle(); // Toggle pause state

#endif // GLOBAL_H