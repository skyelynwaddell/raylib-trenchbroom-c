#ifndef BRUSH_TO_POLYGON_H
#define BRUSH_TO_POLYGON_H

#include <stdbool.h>
#include "defs.h"
#include "raylib.h"
#include "utils.h"
#include "brush.h"
#include "polygon.h"

//https://github.com/stefanha/map-files/blob/master/MAPFiles.pdf

bool polygon_get_intersection(
    Vector3 n1,
    Vector3 n2,
    Vector3 n3,
    double d1,
    double d2,
    double d3,
    Vector3* out
);

void polygon_generate_from_brush(Brush *brush);
void polygon_sort_vertices(Polygon *poly, Vector3 normal);
void polygon_normalize_uv(Vector2 *uvs, int count);
bool polygon_has_vertex(Polygon *poly, Vector3 v);
Vector2 polygon_project_to_uv_standard(Vector3 point, BrushFace *face);
Vector2 polygon_project_to_uv_valve220(Vector3 point, BrushFace *face);

#endif // BRUSH_TO_POLYGON_H