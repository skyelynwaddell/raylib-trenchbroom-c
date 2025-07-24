#include "skyelib.h"
#include "global.h"
#include "player.h"
#include "defs.h"
#include "weapons.h"
#include "anims.h"

static void viewmodel_debug_editor()
{
    float ms = 0.05; // move speed
    if (IsKeyDown(KEY_LEFT)) viewmodel.position.x -= ms; 
    if (IsKeyDown(KEY_RIGHT)) viewmodel.position.x += ms; 
    if (IsKeyDown(KEY_UP)) viewmodel.position.y -= ms; 
    if (IsKeyDown(KEY_DOWN)) viewmodel.position.y += ms; 
    if (IsKeyDown(KEY_LEFT_BRACKET)) viewmodel.position.z += ms;  
    if (IsKeyDown(KEY_RIGHT_BRACKET)) viewmodel.position.z -= ms;

    if (IsKeyDown(KEY_COMMA)) viewmodel.rotation -= ms;
    if (IsKeyDown(KEY_PERIOD)) viewmodel.rotation += ms;

    viewmodel.position.x = Clamp(viewmodel.position.x, -15, 15);
    viewmodel.position.y = Clamp(viewmodel.position.y, -10, 10);
    viewmodel.position.z = Clamp(viewmodel.position.z, -4, 4);

    weapons[current_weapon].position = viewmodel.position;

    //printf("Viewmodel Pos: X:%f Y:%f Z:%f R:%f \n", viewmodel.position.x, viewmodel.position.y, viewmodel.position.z, viewmodel.rotation);

    //if (IsKeyPressed(KEY_BACKSPACE)) viewmodel.position = weapons[current_weapon].position;
}

static void viewmodel_set()
{
    Weapon *w = &weapons[current_weapon];

    viewmodel.model = (sModel){0};
    viewmodel.model = smodel_create(
        w->model_filepath,
        w->model_textures,
        w->anim_count, 
        w->anim_idle,
        camera.position,
        (Vector3){0,0,0},
        w->model_scale
    );

    viewmodel.position = w->position;
    viewmodel.rotation = w->rotation;//-45.0f; // Change this to rotate the model side to side

    for (int i = 0; i < viewmodel.model.model.materialCount; i++)
    {
        // Set the shader for each material
        viewmodel.model.model.materials[i].shader = sh_viewmodel;
    }
}

void viewmodel_init()
{
    viewmodel.camera = (Camera){
    .position = {0.0f, 0.0f, 0.0f},
    .target = {0.0f, 0.0f, -1.0f},   // Looking forward (negative Z)
    .up = {0.0f, 1.0f, 0.0f},
    .fovy = 90.0f,
    .projection = CAMERA_PERSPECTIVE
    };

    viewmodel.rotation_axis = (Vector3){0.0f, 1.0f, 0.0f}; // Y axis

    sh_viewmodel = LoadShader(
        TextFormat("shaders/glsl%i/lighting.vs", GLSL_VERSION),
        TextFormat("shaders/glsl%i/shading.fs", GLSL_VERSION)
    );

    sh_viewmodel.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(sh_viewmodel, "viewPos");
    int ambientLoc_vm = GetShaderLocation(sh_viewmodel, "ambient");
    float str_vm = 0.3f;
    SetShaderValue(sh_viewmodel, ambientLoc_vm, (float[4]){ str_vm, str_vm, str_vm, 10.0f }, SHADER_UNIFORM_VEC4);

    viewmodel_set();
}


/*
draw_viewmodel
-- This function is intended to draw the view model (weapon/arms/etc.) in 3d space
*/
void viewmodel_draw()
{
    Viewmodel *vm = &viewmodel;
    float s = Min((float)GetScreenWidth()/GAME_SCREEN_WIDTH, (float)GetScreenHeight()/GAME_SCREEN_HEIGHT);

    if (VIEWMODEL_POSITION_MODE)
        viewmodel_debug_editor();

    // Draw the weapon model
    DrawModelEx(
        vm->model.model, 
        vm->position, 
        vm->rotation_axis, 
        vm->rotation, 
        (Vector3){1, 1, 1}, 
        WHITE
    );

    if (global_paused) return;
    smodel_animate(&viewmodel.model, true);
}


    