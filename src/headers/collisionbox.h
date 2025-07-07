#ifndef COLLISION_BOX_H
#define COLLISION_BOX_H

#include "raylib.h"

typedef struct {
    Vector3 position;
    Vector3 size;
    int scale;
    BoundingBox bounding_box;
} CollisionBox;

void collisionbox_set(CollisionBox *box, Vector3 _position, Vector3 _size, int _scale);
void collisionbox_update_aabb(CollisionBox *box);
void collisionbox_set_position(CollisionBox *box, Vector3 _position);

#endif // COLLISION_BOX_H