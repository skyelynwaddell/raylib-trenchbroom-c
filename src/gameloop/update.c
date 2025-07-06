#include "../headers/update.h"
#include <camera.h>
#include <global.h>

/*
update
-- this method is called every game tick
-- put your game logic in here
*/
void update()
{
    if (paused) return; // If the game is paused, skip the update logic
    UpdateCamera(&camera, camera_mode);
}