#ifndef LIGHTS_H
#define LIGHTS_H

#include "skyelib.h"
#include "../lib/raylib/include/rlights.h"
#include "player.h"

typedef struct LightObject {
    GameObject gameobject;
    Light light;
    float brightness;
    Color color;
    float radius;
} LightObject;

extern Shader sh_light;
extern float radiusLoc;

void lights_init();
void lights_update();
LightObject light_create(Color color, float brightness, Vector3 position, float radius);

#endif // LIGHTS_H