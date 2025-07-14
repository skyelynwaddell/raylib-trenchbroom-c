#ifndef BRUSH_H
#define BRUSH_H

#include "defs.h"
#include "brushface.h"
#include "polygon.h"

typedef struct {
    int brush_face_count;
    BrushFace brush_faces[BRUSH_FACE_COUNT];
    Polygon polys[BRUSH_FACE_COUNT];
} Brush;


#endif // BRUSH_H