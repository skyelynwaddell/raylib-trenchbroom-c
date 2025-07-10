#include "../headers/global.h"
#include "string.h"
#include "raygui.h"

// game/system properties
int global_paused = false; // Game pause state

// camera properties
float global_cam_yaw = 0.0f;   // left/right
float global_cam_pitch = 0.0f; // up/down

// player globals
Vector3 global_player_spawn = (Vector3){ 8.0f, 3.0f, 7.0f };
int global_player_onground;


/*
pause_toggle
This function toggles the pause state of the game.
*/
void pause_toggle()
{
    global_paused = !global_paused;
    if (global_paused) EnableCursor();
    else DisableCursor();
}


/*
to_delta
Returns value multiplied by Delta Time
*/
float to_delta(float value)
{
    return value * GetFrameTime();
}