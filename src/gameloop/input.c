#include "../headers/input.h"
#include "stdio.h"
#include "global.h"
#include "raylib.h"

/*
input
This function is called every game tick, and handles 
any USER INPUT from keyboard, controllers, etc.
*/
void input()
{
    handle_pause();
}


/*
handle_pause
Pause / Unpause the game when the user presses the ESC key
*/
void handle_pause()
{
    if (IsKeyPressed(KEY_ESCAPE)) 
    {
        pause_toggle();
    }
}