#include "skyelib.h"
#include "global.h"
#include "player.h"


void viewmodel_init()
{
    viewmodel.camera = (Camera){
    .position = {0.0f, 0.0f, 0.0f},
    .target = {0.0f, 0.0f, -1.0f},   // Looking forward (negative Z)
    .up = {0.0f, 1.0f, 0.0f},
    .fovy = 90.0f,
    .projection = CAMERA_PERSPECTIVE
    };

    viewmodel.position = (Vector3){0, -1.8, -1.5};  
    viewmodel.rotation_axis = (Vector3){0.0f, 1.0f, 0.0f}; // Y axis
    viewmodel.rotation = -60.0f; // Change this to rotate the model side to side

    // --- Pistol Textures ---
    viewmodel_pistol_textures[0] = texture_get_cached("skin");
    viewmodel_pistol_textures[1] = texture_get_cached("skin");
    viewmodel_pistol_textures[2] = texture_get_cached("gloves");
    viewmodel_pistol_textures[3] = texture_get_cached("gloves");
    viewmodel_pistol_textures[4] = texture_get_cached("newpistol");
    viewmodel_pistol_textures[5] = texture_get_cached("newpistol");

    viewmodel.model = smodel_create(
        MODEL_PISTOL_ARMS,
        viewmodel_pistol_textures,
        ANIM_COUNT_PISTOL, 
        ANIM_PISTOL_IDLE,
        camera.position,
        (Vector3){0,0,0},
        PISTOL_MODEL_SCALE
    );

    sh_viewmodel = LoadShader(
        TextFormat("shaders/glsl%i/lighting.vs", GLSL_VERSION),
        TextFormat("shaders/glsl%i/shading.fs", GLSL_VERSION)
    );

    sh_viewmodel.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(sh_viewmodel, "viewPos");
    int ambientLoc_vm = GetShaderLocation(sh_viewmodel, "ambient");
    float str_vm = 0.3f;
    SetShaderValue(sh_viewmodel, ambientLoc_vm, (float[4]){ str_vm, str_vm, str_vm, 10.0f }, SHADER_UNIFORM_VEC4);

    for (int i = 0; i < viewmodel.model.model.materialCount; i++)
    {
        // Set the shader for each material
        viewmodel.model.model.materials[i].shader = sh_viewmodel;
    }
}


/*
draw_viewmodel
-- This function is intended to draw the view model (weapon/arms/etc.) in 3d space
*/
void viewmodel_draw()
{
    Viewmodel *vm = &viewmodel;

    // Draw the weapon model
    DrawModelEx(
        vm->model.model, 
        vm->position, 
        vm->rotation_axis, 
        vm->rotation, 
        (Vector3){1.0f, 1.0f, 1.0f}, 
        WHITE
    );

    if (global_paused) return;
    smodel_animate(&viewmodel.model, true);
}


    