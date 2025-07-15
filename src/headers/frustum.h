#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "raylib.h"
#include "raymath.h"
#include "defs.h"
#include "camera.h"

typedef struct {
    Vector4 left, right, top, bottom, near, far;
} Frustum;

Frustum frustum_get_from_camera(Camera3D cam);
int frustum_check_boundingbox(BoundingBox box, Frustum f);

#endif // FRUSTUM_H