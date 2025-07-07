#include "player.h"
#include "raylib.h"
#include <stdio.h>
#include "raymath.h"
#include <math.h>
#include "camera.h"
#include "global.h"
#include "collisionbox.h"

Player player;

/*
player_init
Called when the player object first enters the world
*/
void player_init()
{
    // spawn position
    float p = 0.0f;
    player.gameobject.position = (Vector3){ p,p,p };

    // create player collision box
    float h = 5.0f; // 5 units tall
    float w = 2.0f; // 2 units thick/wide
    collisionbox_set(
        &player.collision_box,
        player.gameobject.position,
        (Vector3){ w, h, w },
        1
    );

    // set player stats
    player.health = 100;
    player.speed = 5.0f;
};


/*
player_update
Called every Tick
*/
void player_update()
{
    if (global_paused) return;

    float move_spd = player.speed * GetFrameTime();

    // Get input directions
    float inputX = (IsKeyDown(KEY_A) - IsKeyDown(KEY_D));
    float inputZ = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));

    // Normalize input direction
    float inputLen = sqrtf(inputX * inputX + inputZ * inputZ);
    if (inputLen > 0.0f) {
        inputX /= inputLen;
        inputZ /= inputLen;
    }

    // Use yaw-only to get movement direction (ignoring pitch)
    Vector3 forward = { sinf(global_cam_yaw), 0.0f, cosf(global_cam_yaw) };       // forward vector
    Vector3 right   = { cosf(global_cam_yaw), 0.0f, -sinf(global_cam_yaw) };      // right vector

    Vector3 moveDir = {
        forward.x * inputZ + right.x * inputX,
        0.0f,
        forward.z * inputZ + right.z * inputX
    };

    moveDir = Vector3Normalize(moveDir);
    moveDir = Vector3Scale(moveDir, move_spd);

    player.gameobject.position = Vector3Add(player.gameobject.position, moveDir);

    //printf("Player Pos x%.2f z%.2f \n", player.gameobject.position.x, player.parent.position.z);
    camera_follow_player(&camera, &player);
    collisionbox_set_position(&player.collision_box, player.gameobject.position);
};


/*
player_draw
Any calls to draw 3d objects on the player can be
ie. collision box shape etc
For drawing weapons, hands, arms etc. see Viewmodel to draw instead
This is for actual player model etc not the arms or view model weapon.
*/
void player_draw()
{
    DrawBoundingBox(player.collision_box.bounding_box, RED);

}