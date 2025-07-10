#ifndef ENTITY_H
#define ENTITY_H
#include "raylib.h"
#include "raymath.h"

typedef struct {
    char classname[64];
    Vector3 origin;
} Entity;

Vector3 convert_entity_origin(Vector3 trench_origin);

#endif // ENTITY_H