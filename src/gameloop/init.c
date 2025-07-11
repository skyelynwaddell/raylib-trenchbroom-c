#include "stdio.h"
#include "../headers/init.h"
#include "../headers/map.h"
#include "../headers/camera.h"
#include "raylib.h"
#include "raygui.h"
#include <defs.h>
#include <global.h>

/*
init
-- the start of the game loop ~~~ 
-- Called ONCE when the game starts!!
*/
void init()
{
    printf("### Skyesrc ###\n");
    camera_init();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
    DisableCursor(); // Limit cursor to relative movement inside the window
    SetTargetFPS(FPS);
    
    map_parse("test.map");
}