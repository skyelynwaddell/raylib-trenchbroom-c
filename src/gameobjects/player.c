#include "player.h"
#include "raylib.h"
#include <stdio.h>
#include "raymath.h"
#include <math.h>
#include "camera.h"
#include "global.h"

Player player;

/*
player_init
Called when the player object first enters the world
*/
void player_init()
{
    player.parent.position = (Vector3){ 0.0f,0.0f,0.0f };
    player.health = 100;
    player.speed = 5.0f;
};


/*
player_update
Called every Tick
*/
void player_update()
{
    if (paused) return;

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

    Vector3 forward = { sinf(cam_yaw), 0.0f, cosf(cam_yaw) };       // forward vector
    Vector3 right   = { cosf(cam_yaw), 0.0f, -sinf(cam_yaw) };      // right vector

    Vector3 moveDir = {
        forward.x * inputZ + right.x * inputX,
        0.0f,
        forward.z * inputZ + right.z * inputX
    };

    moveDir = Vector3Normalize(moveDir);
    moveDir = Vector3Scale(moveDir, move_spd);

    player.parent.position = Vector3Add(player.parent.position, moveDir);


    printf("Player Pos x%.2f z%.2f \n", player.parent.position.x, player.parent.position.z);
    camera_follow_player(&camera, &player);
};