#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "./gameobject.h"

typedef struct {
    GameObject parent;
    int health;
    float speed;
} Player;

extern Player player;

void player_init();
void player_update();

#endif // PLAYER_H