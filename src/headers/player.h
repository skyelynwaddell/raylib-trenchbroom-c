#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "gameobject.h"
#include "collisionbox.h"

typedef struct {
    GameObject gameobject;
    int health;
    int jump_height;
} Player;

extern Player player;

void player_init();
void player_update();
void player_draw();

void player_movement();
void player_jump();
void player_handle_jump();

#endif // PLAYER_H