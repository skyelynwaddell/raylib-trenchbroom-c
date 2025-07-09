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
    player.gameobject.position = global_player_spawn;

    // create player collision box
    float h = 4.0f; // units tall
    float w = 1.0f; // units thick/wide
    int scale = 1;  // collision box scale
    collisionbox_set(
        &player.gameobject.collision_box,
        player.gameobject.position,
        (Vector3){ w, h, w }, 
        scale
    );

    // set player stats
    player.health = 100;
    player.gameobject.speed = 10.0f;
    player.gameobject.gravity = -0.0001f;
};


/*
player_update
Called every Tick
*/
void player_update()
{
    if (global_paused) return;

    float move_spd = to_delta(player.gameobject.speed);
    Vector3 move_dir = player_get_move_dir();

    apply_gravity(&player);
    player_handle_jump();
    check_collisions(&player, move_dir, move_spd, true);
    camera_follow_player(&camera, &player);
}


/*
player_draw
Any calls to draw 3d objects on the player can be put here
ie. collision box shape, 3rd person player model, etc
NOTE: For drawing weapons, hands, arms etc. see Viewmodel to draw instead
*/
void player_draw()
{
    DrawBoundingBox(player.gameobject.collision_box.bounding_box, RED);
}


/*
player_get_move_dir
Calculates the players movement direction from the control inputs
*/
Vector3 player_get_move_dir()
{
    // get intial input values
    float input_x = (IsKeyDown(KEY_A) - IsKeyDown(KEY_D));
    float input_z = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));

    // move in the camera direction
    Vector3 forward = { sinf(global_cam_yaw), 0.0f, cosf(global_cam_yaw) };
    Vector3 right   = { cosf(global_cam_yaw), 0.0f, -sinf(global_cam_yaw) };

    // calculate final move val
    Vector3 move_dir = {
        forward.x * input_z + right.x * input_x,
        0.0f,
        forward.z * input_z + right.z * input_x
    };

    // normalize speed when we are moving forward and horizontal
    float len = Vector3Length(move_dir);
    if (len > 0.0f) move_dir = Vector3Scale(move_dir, 1.0f / len);

    return move_dir;
}


/*
player_jump
Call this to invoke the player jumping into the air
*/
void player_jump()
{
    player.gameobject.yspd = 0.02f;
    global_player_onground = false;
}


/*
player_handle_jump
Called to handle when / if the player CAN jump
*/
void player_handle_jump()
{
    // jump
    if (IsKeyPressed(KEY_SPACE) && global_player_onground)
        player_jump();
}