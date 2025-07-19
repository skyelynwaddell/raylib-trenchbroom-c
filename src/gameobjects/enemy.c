#include "skyelib.h"
#include "enemy.h"
#include "enemy_shotgunner.h"
Enemy *enemies[MAX_ENEMIES] = {0};  // Stores all enemies in memory

/*
*enemy_create()
Returns a pointer to a new enemy object
*/
Enemy *enemy_create(ENEMY_TYPE type, Vector3 position)
{
    Enemy *enemy = NULL;


    switch (type) {
        case ENEMY_SHOTGUNNER: enemy = shotgunner_create(position); break;
        default: break;
    }

    // Add to global enemy list
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i]) {
            enemies[i] = enemy;
            break;
        }
    }

    enemy->gameobject.gravity = GRAVITY_DEFAULT;
    enemy->gameobject.visible = true;

    return enemy;
}


/*
enemy_update_all
Called every TICK to call all the Enemies Update script
*/
void enemy_update_all()
{
    if (global_game_loading) return;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i]) {
            enemies[i]->update(enemies[i]);
        }
    }
}


/*
enemy_draw_all()
Called every TICK to call all the Enemies Draw script
*/
void enemy_draw_all()
{
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i] && enemies[i]->draw) {
            enemies[i]->draw(enemies[i]);
        }
    }
}


/*
enemy_cleanup_all()
Cleans up all the enmies models, and textures from memory
*/
void enemy_cleanup_all()
{
    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = enemies[i];
        if (e) {
            // Unload model and animation if needed
            UnloadModel(e->model.model);

            if (e->model.anims && e->model.anim_count > 0) {
                UnloadModelAnimations(e->model.anims, e->model.anim_count);
            }

            free(e);
            enemies[i] = NULL;
        }
    }
}

/*
enemy_change_state()
Call this to change an enemies current state
and switch its animation
*/
void enemy_change_state(Enemy *mon, int new_state, int new_anim)
{
    mon->state = new_state;
    mon->gameobject.is_hit = false;
    smodel_animation_change(&mon->model, new_anim);
}


int enemy_should_update(Enemy *mon)
{
    if (mon->gameobject.visible == false || distance_to_player(&mon->gameobject, RENDER_DISTANCE) == false) return false;
    if (mon->health <= 0 && mon->model.current_anim_finished == true) return false;
    return true;
}