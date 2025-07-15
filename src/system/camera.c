#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "camera.h"
#include "defs.h"
#include "global.h"

Camera3D camera;
int camera_mode;
int camera_move_spd;

/*
camera_init
Creates the 3d camera object
*/
void camera_init()
{
    camera.position = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera_mode = CAMERA_FIRST_PERSON;
}


/*
camera_follow_player
Makes the Camera follow the Player target in First Person
*/
void camera_follow_player(Camera3D *camera, Player *target)
{
    Vector2 mouseDelta = GetMouseDelta();
    float sensitivity = 0.002f;

    global_cam_yaw -= mouseDelta.x * sensitivity;
    global_cam_pitch -= mouseDelta.y * sensitivity;

    // Clamp pitch between looking straight up and down
    if (global_cam_pitch > PI/2 - 0.01f) global_cam_pitch = PI/2 - 0.01f;
    if (global_cam_pitch < -PI/2 + 0.01f) global_cam_pitch = -PI/2 + 0.01f;

    // Calculate forward vector from yaw and pitch
    Vector3 forward = {
        cosf(global_cam_pitch) * sinf(global_cam_yaw),
        sinf(global_cam_pitch),
        cosf(global_cam_pitch) * cosf(global_cam_yaw)
    };

    // Set camera position just above player position (eye height)
    Vector3 eyeOffset = {0.0f, 1.7f, 0.0f}; // adjust for player height
    camera->position = Vector3Add(target->gameobject.position, eyeOffset);

    // Camera target is forward direction from eye position
    camera->target = Vector3Add(camera->position, forward);

    // Up vector stays world up
    camera->up = (Vector3){0.0f, 1.0f, 0.0f};

    // Optionally update player's yaw (to rotate player model with camera)
    // target->parent.rotation.y = yaw; // if your player has rotation property
}


/*
camera_get_forward
Gets the forward direction of the camera
*/
Vector3 camera_get_forward(Camera *camera)
{
    return Vector3Normalize(Vector3Subtract(camera->target, camera->position));
}


/*
camera_get_up
Returns the cameras up vector (normalized)
Note: The up vector might not be perpendicular to the forward vector
*/
Vector3 camera_get_up(Camera *camera)
{
    return Vector3Normalize(camera->up);
}


/*
camera_get_right
Returns the cameras right vector (normalized)
*/
Vector3 camera_get_right(Camera *camera)
{
    Vector3 forward = camera_get_forward(camera);
    Vector3 up = camera_get_up(camera);

    return Vector3Normalize(Vector3CrossProduct(forward, up));
}


/*
camera_yaw
Rotates the camera around its up vector
Yaw is "looking left and right"
If rotateAroundTarget is false, the camera rotates around its position
Note: angle must be provided in radians
*/
void camera_yaw(Camera *camera, float angle, bool rotateAroundTarget)
{
    // Rotation axis
    Vector3 up = camera_get_up(camera);

    // View vector
    Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);

    // Rotate view vector around up axis
    targetPosition = Vector3RotateByAxisAngle(targetPosition, up, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        camera->position = Vector3Subtract(camera->target, targetPosition);
    }
    else // rotate around camera.position
    {
        // Move target relative to position
        camera->target = Vector3Add(camera->position, targetPosition);
    }
}


/*
camera_pitch
Rotates the camera around its right vector, pitch is "looking up and down"
lockView prevents camera overrotation (aka "somersaults")
rotateAroundTarget defines if rotation is around target or around its position
rotateUp rotates the up direction as well (typically only usefull in CAMERA_FREE)
NOTE: angle must be provided in radians
*/
void camera_pitch(
    Camera *camera, 
    float angle, 
    bool lockView, 
    bool rotateAroundTarget, 
    bool rotateUp
){
    // Up direction
    Vector3 up = camera_get_up(camera);

    // View vector
    Vector3 targetPosition = Vector3Subtract(camera->target, camera->position);

    if (lockView)
    {
        // In these camera modes we clamp the Pitch angle
        // to allow only viewing straight up or down.

        // Clamp view up
        float maxAngleUp = Vector3Angle(up, targetPosition);
        maxAngleUp -= 0.001f; // avoid numerical errors
        if (angle > maxAngleUp) angle = maxAngleUp;

        // Clamp view down
        float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
        maxAngleDown *= -1.0f; // downwards angle is negative
        maxAngleDown += 0.001f; // avoid numerical errors
        if (angle < maxAngleDown) angle = maxAngleDown;
    }

    // Rotation axis
    Vector3 right = camera_get_right(camera);

    // Rotate view vector around right axis
    targetPosition = Vector3RotateByAxisAngle(targetPosition, right, angle);

    if (rotateAroundTarget)
    {
        // Move position relative to target
        camera->position = Vector3Subtract(camera->target, targetPosition);
    }
    else // rotate around camera.position
    {
        // Move target relative to position
        camera->target = Vector3Add(camera->position, targetPosition);
    }

    if (rotateUp)
    {
        // Rotate up direction around right axis
        camera->up = Vector3RotateByAxisAngle(camera->up, right, angle);
    }
}


/*
camera_roll
Rotates the camera around its forward vector
Roll is "turning your head sideways to the left or right"
Note: angle must be provided in radians
*/
void camera_roll(Camera *camera, float angle)
{
    // Rotation axis
    Vector3 forward = camera_get_forward(camera);

    // Rotate up direction around forward axis
    camera->up = Vector3RotateByAxisAngle(camera->up, forward, angle);
}