#ifndef POLYGON_H
#define POLYGON_H

#include "raylib.h"
#include "defs.h"

typedef struct {
    Vector3 vertices[MAX_VERTICES_PER_FACE];
    int vertex_count;
} Polygon;

#endif // POLYGON_H