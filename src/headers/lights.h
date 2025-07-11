#ifndef LIGHTS_H
#define LIGHTS_H

#include "raylib.h"
#include "raymath.h"
#include "../lib/raylib/include/rlights.h"
#include "gameobject.h"
#include "player.h"
#include "camera.h"
#include "defs.h"

typedef struct {
    GameObject gameobject;
    Light light;
    float brightness;
    Color color;
    float radius;
} LightObject;

Shader sh_light;
LightObject lights[MAX_LIGHTS];
float radiusLoc;

void lights_init();
void lights_update();
LightObject light_create(Color color, float brightness, Vector3 position, float radius);

#endif // LIGHTS_H