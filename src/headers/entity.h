#ifndef ENTITY_H
#define ENTITY_H
#include "raylib.h"
#include "raymath.h"

typedef struct {
    char classname[64];
    Vector3 origin;

    // light properties
    Color color;
    float brightness;
    float radius;


} Entity;

#endif // ENTITY_H