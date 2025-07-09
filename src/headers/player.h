#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "./gameobject.h"
#include "./collisionbox.h"

typedef struct {
    GameObject gameobject;
    int health;
} Player;

extern Player player;

void player_init();
void player_update();
void player_draw();

Vector3 player_get_move_dir();
void player_jump();
void player_handle_jump();

#endif // PLAYER_H