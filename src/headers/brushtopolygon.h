#ifndef BRUSH_TO_POLYGON_H
#define BRUSH_TO_POLYGON_H

#include <stdbool.h>
#include "raylib.h"

#include "./brush.h"
#include "./polygon.h"

//https://github.com/stefanha/map-files/blob/master/MAPFiles.pdf

#define MAX_POLYGONS 64

bool polygon_get_intersection(
    Vector3 n1,
    Vector3 n2,
    Vector3 n3,
    float d1,
    float d2,
    float d3,
    Vector3* out
);

void polygon_generate_from_brush(Brush *brush);
void polygon_sort_vertices(Polygon* poly);
void polygon_draw_all();

#endif // BRUSH_TO_POLYGON_H