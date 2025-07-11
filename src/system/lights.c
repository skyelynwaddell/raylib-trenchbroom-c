
#define RLIGHTS_IMPLEMENTATION
#define PLATFORM_DESKTOP

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#include "lights.h"

Shader sh_light;
LightObject lights[MAX_LIGHTS] = {0};
float radiusLoc;

/*
light_create
Creates a light object with the designated properties
Illuminates the world and gameobjects
*/
LightObject light_create(
    Color color, 
    float brightness, 
    Vector3 position, 
    float radius
){
    LightObject l;
    l.brightness = brightness;
    l.radius = radius;

    // Apply brightness to color (scale and clamp to 255)
    Color scaled = {
        (unsigned char)fminf(color.r * brightness, 255.0f),
        (unsigned char)fminf(color.g * brightness, 255.0f),
        (unsigned char)fminf(color.b * brightness, 255.0f),
        color.a
    };

    l.color = scaled;

    l.light = CreateLight(
        LIGHT_POINT, 
        position,
        Vector3Zero(), 
        scaled, 
        sh_light);

    return l;
}


/*
lights_init
Called on room entry once
Will initialize all the lights in a room
*/
void lights_init()
{
    sh_light = LoadShader(TextFormat("shaders/glsl%i/lighting.vs", GLSL_VERSION),
                                 TextFormat("shaders/glsl%i/lighting.fs", GLSL_VERSION));
    sh_light.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(sh_light, "viewPos");
    int ambientLoc = GetShaderLocation(sh_light, "ambient");
    
    float str = 0.2f; // how dark it can get (0 = BLACK)
    SetShaderValue(sh_light, ambientLoc, (float[4]){ str, str, str, 10.0f }, SHADER_UNIFORM_VEC4);
    lights[0] = light_create(WHITE, 0.6f, player.gameobject.position, 40.0f);
    radiusLoc = GetShaderLocation(sh_light, "radius");

}


/*
lights_update
Updates all the lights properties
Including brightness, color, radius.
NOTE: Must be called every TICK or else lights will not work
*/
void lights_update()
{
    float cam_pos[3] = { camera.position.x,camera.position.y,camera.position.z };
    SetShaderValue(sh_light, sh_light.locs[SHADER_LOC_VECTOR_VIEW], cam_pos, SHADER_UNIFORM_VEC3);
    for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(sh_light, lights[i].light);

    float radii[MAX_LIGHTS] = { 0 };
    for (int i = 0; i < MAX_LIGHTS; i++) {
        radii[i] = lights[i].radius;
    }
    SetShaderValue(sh_light, radiusLoc, radii, SHADER_UNIFORM_FLOAT);
}
