#ifndef COLLISION_POLYGON_H
#define COLLISION_POLYGON_H

#include "triangle.h"

typedef struct {
    Triangle *triangles;
    int count;
} CollisionPolygon;

#endif // COLLISION_POLYGON_H