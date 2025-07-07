#include "../headers/global.h"
#include "string.h"
#include "raygui.h"

int paused = false; // Game pause state

float cam_yaw = 0.0f;   // left/right
float cam_pitch = 0.0f; // up/down


/*
pause_toggle
This function toggles the pause state of the game.
*/
void pause_toggle()
{
    paused = !paused;
    if (paused) EnableCursor();
    else DisableCursor();
}