#include "../headers/global.h"
#include "string.h"
#include "raygui.h"

int paused = false; // Game pause state

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