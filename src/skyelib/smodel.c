#include "skyelib.h"
#include "global.h"

int ANIM_SPEED = 60;

/*
smodel_create()
Creates a new model to be attached to a Character ie (player, enemy, etc.)
*/
sModel smodel_create(
    char model_filepath[255], 
    Texture textures[64],
    int anim_count,
    int current_anim,
    Vector3 position,
    Vector3 rotation,
    float scale
){
    sModel model = {0};

    char fullpath[512] = {0};
    snprintf(fullpath, sizeof(fullpath), "%s%s", MODEL_DIR, model_filepath);

    model.model = LoadModel(fullpath);

    // copy file path
    strncpy(model.model_filepath, fullpath, sizeof(model.model_filepath) - 1);
    model.model_filepath[sizeof(model.model_filepath) - 1] = '\0';

    // copy textures array
    memcpy(model.textures, textures, sizeof(Texture) * 64);

    // set animation properties
    model.anim_count = anim_count;
    model.current_anim = current_anim;
    model.anims = LoadModelAnimations(fullpath, &anim_count);

    // model position
    model.position = position;
    model.scale = scale;
    model.rotation = rotation;

    // make the model affected by light
    for (int i=0; i < model.model.materialCount; i++)
    {
        printf("Material count ; %d\n", model.model.materialCount);
        model.model.materials[i].shader = sh_light;
        model.model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = textures[i];
    }

    return model;
}


/*
smodel_draw()
Draws a model in 3d space that was created with smodel_create
exluded mesh, allows you to fill an array with numbers, and excluded count is how many you put in there
It will search the model for a matching Mesh ID and will NOT draw it.
Useful for gibs & limb removals, dropping weapons, etc.
-- Set excluded_mesh = NULL to draw entire model
-- set excluded_count to 0 to draw entire model
*/
void smodel_draw(sModel *model, int excluded_mesh[255], int excluded_count)
{
    // Loop though the models meshes
    for (int i = 0; i < model->model.meshCount; i++)
    {
        int excluded = false;

        // check if we should exclude this mesh from drawing
        if (excluded_mesh != NULL && excluded_count > 0)
        {
            // loop through all mesh ID's that should be excluded
            for (int j=0; j<excluded_count; j++)
            {
                // found an excluded mesh that matched the 
                // current mesh we were about to render
                if (excluded_mesh[j] == i) 
                {
                    excluded = true;
                    break;
                }
            }
        }

        // dont draw this mesh if it should be excluded
        if (excluded == true) continue;
        
        Mesh mesh = model->model.meshes[i];
        Material material = model->model.materials[model->model.meshMaterial[i]];
        DrawMesh(mesh, material, model->model.transform);
    }
}


/*
smodel_animate
Animates the current sModel model
*/
void smodel_animate(sModel *model, int loop)
{
    if (global_paused) return;
    if (loop == false && model->current_anim_finished == true) return;
    float delta = GetFrameTime();

    model->current_frame += delta * ANIM_SPEED;

    if (model->current_frame >= 
        model->anims[model->current_anim].frameCount)
    {
        if (loop == false)
        {
            model->current_frame = 
                model->anims[model->current_anim].frameCount-1;

        }
            model->current_anim_finished = true;
    }
    smodel_update_animation(model);
}


/*
smodel_animation_change()
Call this to change an animation of a model
while correctly reseting it to the beginning 
of the animation, and unsetting the anim finished
*/
void smodel_animation_change(sModel *model, int new_anim)
{   
    model->current_anim = new_anim;
    model->current_frame = 0;
    model->current_anim_finished = false;
}


/*
smodel+update_animation()
Called to update an enmies animation
Without this they probably wont be animated just saying
*/
void smodel_update_animation(sModel *model)
{
    // Update model animation
    UpdateModelAnimation(model->model, model->anims[model->current_anim], model->current_frame);
}


// /*
// smodel_update_position()
// Call this to make sure the model follows the gameobjects position
// */
void smodel_update_position(GameObject *obj, sModel *model, Vector3 offset)
{
    Vector3 pos = {
        obj->position.x + offset.x,
        obj->position.y + offset.y,
        obj->position.z + offset.z
    };

    float s = model->scale;
    Matrix scale = MatrixScale(s, s, s);
    Matrix translate = MatrixTranslate(pos.x, pos.y, pos.z);

    // Apply scale then translation: scale * translate
    model->model.transform = MatrixMultiply(scale, translate);
}
