#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "raylib.h"
#include "collisionbox.h"
#include "collisiontriangle.h"

typedef struct {
    Vector3 position;
    Vector3 velocity;
    CollisionBox collision_box;
    float speed;
    float gravity;
    float xspd;
    float yspd;
    
} GameObject;

int place_meeting_solid(GameObject *object);
int CheckCollisionBoxesExt(BoundingBox, CollisionTriangle shape);
int check_AABB_triangle_SAT(BoundingBox box, Triangle tri);
void apply_gravity(GameObject *obj);
void check_collisions(GameObject *obj, int is_player);

#endif // GAMEOBJECT_H