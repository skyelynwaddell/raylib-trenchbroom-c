#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "raylib.h"
#include "collisiontriangle.h"

typedef struct {
    Model model;
    BoundingBox bounds;
    CollisionTriangle collision;
} Geometry;



#endif // GEOMETRY_H