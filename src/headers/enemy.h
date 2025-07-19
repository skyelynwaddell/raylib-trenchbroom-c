#ifndef ENEMY_H
#define ENEMY_H

#include "skyelib.h"
#include "global.h"
#include "anims.h"
#include "player.h"

typedef enum ENEMY_TYPE {
    ENEMY_SHOTGUNNER
} ENEMY_TYPE;

typedef struct Enemy {
    ENEMY_TYPE type;
    GameObject gameobject;
    sModel model;

    int health;
    ENEMY_STATE state;

    void (*update)(void *self); 
    void (*draw)(void *self);    
} Enemy;

Enemy *enemies[MAX_ENEMIES];

Enemy *enemy_create(ENEMY_TYPE type, Vector3 position);
void enemy_update_all();
void enemy_draw_all();
void enemy_cleanup_all();
void enemy_change_state(Enemy *mon, int new_state, int new_anim);
int enemy_should_update(Enemy *mon);
#endif // ENEMY_H