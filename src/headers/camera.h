#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include "raylib.h"
#include "player.h"
#include "plane.h"

extern Camera camera;
extern int camera_mode;
extern int camera_move_spd;

void camera_init();
void camera_follow_player(Camera3D *camera, Player *target);

Vector3 camera_get_forward(Camera *camera);
Vector3 camera_get_up(Camera *camera);
Vector3 camera_get_right(Camera *camera);

void camera_yaw(Camera *camera, float angle, bool rotateAroundTarget);
void camera_pitch(Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp);
void camera_roll(Camera *camera, float angle);


#endif