#include "gameloop.h"
#include "skyelib.h"
#include "enemy.h"

/*
clean_up
-- clean up event called when the game ends
-- and in between level changes -- possibly portal changes
-- put any garbage collection in here
*/
void clean_up()
{
    map_clear_models(); //clears all the map models cache from memory
    UnloadShader(sh_light);
    UnloadShader(sh_viewmodel);
    enemy_cleanup_all();
    texture_cache_cleanup();
}