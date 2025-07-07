#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "./gameobject.h"
#include "./collisionbox.h"

typedef struct {
    GameObject gameobject;
    CollisionBox collision_box;
    int health;
    float speed;
} Player;

extern Player player;

void player_init();
void player_update();
void player_draw();

#endif // PLAYER_H