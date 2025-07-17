#include "skyelib.h"
#include "gameloop.h"
#include "global.h"
#include "map.h"

/*
update
-- this method is called every game tick
-- put your game logic in here
*/
void update()
{
    if (global_paused) return; // If the game is paused, skip the update logic
    map_hotreload();
    //UpdateCamera(&camera, camera_mode);
}