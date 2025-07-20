#include "skyelib.h"
#include "global.h"

#define MODEL_PISTOL_ARMS "arms_newpistol/arms_newpistol.glb"
static Texture viewmodel_pistol_textures[6];
static float PISTOL_MODEL_SCALE = 1.2f;
static int ANIM_COUNT_PISTOL = 6;

static int ANIM_PISTOL_IDLE = 0;
static int ANIM_PISTOL_RELOAD = 2;

void viewmodel_init()
{
    // Pistol Textures
    viewmodel_pistol_textures[0] = texture_get_cached("skin");
    viewmodel_pistol_textures[1] = texture_get_cached("skin");
    viewmodel_pistol_textures[2] = texture_get_cached("gloves");
    viewmodel_pistol_textures[3] = texture_get_cached("gloves");
    viewmodel_pistol_textures[4] = texture_get_cached("newpistol");
    viewmodel_pistol_textures[5] = texture_get_cached("newpistol");
    
    view_model = smodel_create(
        MODEL_PISTOL_ARMS,
        viewmodel_pistol_textures,
        ANIM_COUNT_PISTOL, 
        ANIM_PISTOL_IDLE,
        camera.position,
        (Vector3){0,0,0},
        PISTOL_MODEL_SCALE,
        (LightObject){0}
    );

}


/*
draw_viewmodel
-- This function is intended to draw the view model (weapon/arms/etc.) in 3d space
*/
void viewmodel_draw(Camera *weaponCamera)
{
    
    Vector3 weaponPosition = {0, -1.8, -1.5};  
    Vector3 rotationAxis = {0.0f, 1.0f, 0.0f}; // Y axis
    float angle = -60.0f;                            // Static rotation for the weapon

    // Draw the weapon model
    DrawModelEx(view_model.model, weaponPosition, rotationAxis, angle, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);

    if (global_paused) return;
    smodel_animate(&view_model, true);
}


    