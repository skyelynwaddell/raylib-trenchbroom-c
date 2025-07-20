#include "gameloop.h"
#include "skyelib.h"
#include "global.h"

/*
update
-- this method is called every game tick
-- put your game logic in here
*/
void update()
{
    if (global_paused) return; // If the game is paused, skip the update logic
    map_hotreload();
    console_update();
    global_raycast_has_target = false;
}