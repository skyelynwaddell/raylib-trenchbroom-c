#include "../headers/global.h"
#include "string.h"
#include "raygui.h"

int global_paused = false; // Game pause state
float global_cam_yaw = 0.0f;   // left/right
float global_cam_pitch = 0.0f; // up/down

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