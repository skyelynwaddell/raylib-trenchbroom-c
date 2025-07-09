#include "gameobject.h"
#include "geometry.h"
#include "raymath.h"
#include "map.h"
#include "global.h"

/*
place_meeting
returns TRUE if GameObject is colliding with a solid geometry / wall.
*/
int place_meeting_solid(GameObject *object)
{
    for (int i=0; i < model_count; i++)
    {
    // potential collision -- do cheap math first
    if ((CheckCollisionBoxes(object->collision_box.bounding_box, models[i].bounds)) == false) 
        continue;

    //actual collision -- do expensive math
    if (CheckCollisionBoxesExt(object->collision_box.bounding_box, models[i].collision))
        return true;

    }
    return false;
}


/*
CheckCollisionBoxesExt
Checks for a collision between a BoundingBox and a CollisionTriangle
For expensive calculations against players simple BoundingBox and a Polygonal Brush/Geometry in the world
*/
int CheckCollisionBoxesExt(BoundingBox box, CollisionTriangle shape)
{
    for (int i=0; i<shape.count; i++)
    {
        if (check_AABB_triangle_SAT(box, shape.triangles[i])) 
            return true;
    }
    return false;
}


/*
check_AABB_triangle_SAT
Used to check collision between BoundingBox and a Triangle
*/
int check_AABB_triangle_SAT(BoundingBox box, Triangle tri)
{
    // get triangle vertices relative to box center
    Vector3 boxCenter = Vector3Scale(Vector3Add(box.min, box.max), 0.5f);
    Vector3 boxHalfSize = Vector3Subtract(box.max, boxCenter);

    Vector3 v0 = Vector3Subtract(tri.a, boxCenter);
    Vector3 v1 = Vector3Subtract(tri.b, boxCenter);
    Vector3 v2 = Vector3Subtract(tri.c, boxCenter);

    Vector3 f0 = Vector3Subtract(v1, v0);
    Vector3 f1 = Vector3Subtract(v2, v1);
    Vector3 f2 = Vector3Subtract(v0, v2);

    // 13 possible separating axes
    Vector3 axes[13] = {
        // box normals (x, y, z)
        {1,0,0}, {0,1,0}, {0,0,1},
        // cross products of edges
        Vector3CrossProduct((Vector3){1,0,0}, f0),
        Vector3CrossProduct((Vector3){1,0,0}, f1),
        Vector3CrossProduct((Vector3){1,0,0}, f2),
        Vector3CrossProduct((Vector3){0,1,0}, f0),
        Vector3CrossProduct((Vector3){0,1,0}, f1),
        Vector3CrossProduct((Vector3){0,1,0}, f2),
        Vector3CrossProduct((Vector3){0,0,1}, f0),
        Vector3CrossProduct((Vector3){0,0,1}, f1),
        Vector3CrossProduct((Vector3){0,0,1}, f2),
        // triangle normal
        Vector3CrossProduct(f0, f1)
    };

    for (int i = 0; i < 13; i++)
    {
        Vector3 axis = axes[i];
        if (Vector3Length(axis) < 0.0001f) continue; // skip near-zero axes

        // project triangle onto axis
        float minT = INFINITY, maxT = -INFINITY;
        float dots[3] = {
            Vector3DotProduct(v0, axis),
            Vector3DotProduct(v1, axis),
            Vector3DotProduct(v2, axis)
        };

        for (int j = 0; j < 3; j++) {
            if (dots[j] < minT) minT = dots[j];
            if (dots[j] > maxT) maxT = dots[j];
        }

        // project box onto axis
        float r =
            boxHalfSize.x * fabsf(Vector3DotProduct(axis, (Vector3){1,0,0})) +
            boxHalfSize.y * fabsf(Vector3DotProduct(axis, (Vector3){0,1,0})) +
            boxHalfSize.z * fabsf(Vector3DotProduct(axis, (Vector3){0,0,1}));

        // SAT test
        if (maxT < -r || minT > r) {
            return false; // separation found - NO collision
        }
    }

    return true; // no separation axis found — collision!
}


/*
player_apply_gravity
Applies vertical gravity to the player object
*/
void apply_gravity(GameObject *obj)
{
    // apply gravity
    obj->yspd += obj->gravity;
}


/*
check_collisions
Handles all the X Y Z collisions between obj and walls/floors/ceilings
*/
void check_collisions(GameObject *obj, Vector3 move_dir, float move_spd, int is_player)
{
    // check for Floor collisions
    // --- try Y movement ---
    Vector3 try_move_y = { 0.0f, obj->yspd, 0.0f };
    Vector3 test_pos_y = Vector3Add(obj->position, try_move_y);
    collisionbox_set_position(&obj->collision_box, test_pos_y);

    if (!place_meeting_solid(obj)) {
        // falling
        obj->position.y += obj->yspd;
        
        if (is_player == true) 
            global_player_onground = false;
    } 
    else 
    {
        // on floor
        if (obj->yspd < 0.0f && is_player == true) 
            global_player_onground = true;

        obj->yspd = 0.0f;
    }

    // check for wall collisions
    if (Vector3Length(move_dir) > 0.0f)
    {
        move_dir = Vector3Normalize(move_dir);
        move_dir = Vector3Scale(move_dir, move_spd);

        Vector3 original_pos = obj->position;

        // --- try X movement ---
        Vector3 try_move_x = { move_dir.x, 0.0f, 0.0f };
        Vector3 test_pos_x = Vector3Add(original_pos, try_move_x);
        collisionbox_set_position(&obj->collision_box, test_pos_x);
        if (!place_meeting_solid(obj)) {
            obj->position.x += move_dir.x;
        }

        // --- try Z movement ---
        Vector3 try_move_z = { 0.0f, 0.0f, move_dir.z };
        Vector3 test_pos_z = Vector3Add(original_pos, (Vector3){obj->position.x - original_pos.x, 0.0f, 0.0f});
        test_pos_z = Vector3Add(test_pos_z, try_move_z);
        collisionbox_set_position(&obj->collision_box, test_pos_z);
        if (!place_meeting_solid(obj)) {
            obj->position.z += move_dir.z;
        }
    }
    // set final collision box position
    collisionbox_set_position(&obj->collision_box, obj->position);
}