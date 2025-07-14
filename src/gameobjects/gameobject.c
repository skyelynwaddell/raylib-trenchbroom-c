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
    for (int i=0; i < map.model_count; i++)
    {
    // potential collision -- do cheap math first
    if ((CheckCollisionBoxes(object->collision_box.bounding_box, map.models[i].bounds)) == false) 
        continue;

    //actual collision -- do expensive math
    if (CheckCollisionBoxesExt(object->collision_box.bounding_box, map.models[i].collision))
        return true;

    }
    return false;
}


/*
CheckCollisionBoxesExt
Checks for a collision between a BoundingBox and a CollisionPolygon
For expensive calculations against players simple BoundingBox and a Polygonal Brush/Geometry in the world
*/
int CheckCollisionBoxesExt(BoundingBox box, CollisionPolygon shape)
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
    obj->velocity.y += obj->gravity;
}


/*
check_collisions
Handles all the X Y Z collisions between obj and walls/floors/ceilings
*/
void check_collisions(GameObject *obj, int is_player)
{
    Vector3 original_pos = obj->position;
    float dt = GetFrameTime();

    // ---- Y axis (gravity + jump) ----
    Vector3 moveY = { 0.0f, obj->velocity.y * dt, 0.0f };
    Vector3 testY = Vector3Add(obj->position, moveY);
    collisionbox_set_position(&obj->collision_box, testY);

    if (!place_meeting_solid(obj)) {
        obj->position.y += moveY.y;
        if (is_player) global_player_onground = false;
    } else {
        if (obj->velocity.y < 0 && is_player) global_player_onground = true;
        obj->velocity.y = 0.0f;
    }

    // ---- X axis ----
    Vector3 moveX = { obj->velocity.x * dt, 0.0f, 0.0f };
    Vector3 testX = Vector3Add(obj->position, moveX);
    collisionbox_set_position(&obj->collision_box, testX);

    if (!place_meeting_solid(obj)) {
        obj->position.x += moveX.x;
    } else {
        obj->velocity.x = 0.0f;
    }

    // ---- Z axis ----
    Vector3 moveZ = { 0.0f, 0.0f, obj->velocity.z * dt };
    Vector3 testZ = Vector3Add(obj->position, moveZ);
    collisionbox_set_position(&obj->collision_box, testZ);

    if (!place_meeting_solid(obj)) {
        obj->position.z += moveZ.z;
    } else {
        obj->velocity.z = 0.0f;
    }

    // Final update of the collision box
    collisionbox_set_position(&obj->collision_box, obj->position);
}
