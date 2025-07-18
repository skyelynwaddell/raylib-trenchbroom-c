#include "stdio.h"
#include "init.h"
#include "map.h"
#include "camera.h"
#include "raylib.h"
#include <defs.h>
#include <global.h>
#include "lights.h"

/*
init
-- the start of the game loop ~~~ 
-- Called ONCE when the game starts!!
*/
void init()
{
    printf("\n \n ### SKYESRC ### \n \n");
    camera_init();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
    DisableCursor(); // Limit cursor to relative movement inside the window
    SetTargetFPS(FPS);
    
    lights_init();
    map_parse("test.map");
}