#ifndef BRUSHFACE_H
#define BRUSHFACE_H

#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "string.h"
#include "plane.h"

typedef struct {

    // Defines the position, size, and directions of the infinite plane
    Vector3 pos_1;    // [ x y z ] Origin Point of the plane --- The anchor point / position of the plane
    Vector3 pos_2;    // [ x y z ] with pos_1 defines the first infinite direction of the plane
    Vector3 pos_3;    // [ x y z ] with pos_1 defines the second infinite direction of the plane

    // with all the infinite planes in a brush we can clip all the geometry wherever 
    // any of the infinite planes intersect with eachother to form the convex polygon.
    
    // texture data for a brush face
    char texture[64]; // texture string name (not including filetype)

    Vector4 uv_s; // [ Ux Uy Uz Uoffset ]
    Vector4 uv_t; // [ Vx Vy Vz Voffset ]

    int uv_rotation; // texture rotation degrees
    int u_scale;     // horizontal texture scale
    int v_scale;     // vertical texture scale

} BrushFace;

void brushface_print(BrushFace b, int face_index);
Plane brushface_to_plane(BrushFace face);

#endif // BRUSHFACE_H