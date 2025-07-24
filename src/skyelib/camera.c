#include "skyelib.h"
#include "global.h"
#include "weapons.h"

Camera3D camera;
int camera_mode;
int camera_move_spd;

float camera_bob_phase = 0.0f;
float camera_bob_amp = 0.02f;
float camera_bob_speed = 8.4f;
float bob_offset = 0.0f;

float weapon_bob = 0.006f;
float camera_tilt = 0.0f;
float camera_tilt_max = 0.01f; 
float camera_tilt_speed = 8.0f;

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
void camera_follow_player(Camera3D *camera, GameObject *target)
{
    Vector2 mouseDelta = GetMouseDelta();
    float sensitivity = 0.002f;

    global_cam_yaw -= mouseDelta.x * sensitivity;
    global_cam_pitch -= mouseDelta.y * sensitivity;

    if (IsGamepadAvailable(GAMEPAD_P1)) // GAMEPAD_PLAYER1
    {
        float look_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
        float look_y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

        float deadzone = 0.2f;
        if (fabsf(look_x) < deadzone) look_x = 0.0f;
        if (fabsf(look_y) < deadzone) look_y = 0.0f;

        float joystick_sensitivity = 0.006f;

        global_cam_yaw   -= look_x * joystick_sensitivity;
        global_cam_pitch -= look_y * joystick_sensitivity;
    }

    // Clamp pitch between looking straight up and down
    if (global_cam_pitch > PI/2 - 0.01f) global_cam_pitch = PI/2 - 0.01f;
    if (global_cam_pitch < -PI/2 + 0.01f) global_cam_pitch = -PI/2 + 0.01f;

    // Calculate forward vector from yaw and pitch
    Vector3 forward = {
        cosf(global_cam_pitch) * sinf(global_cam_yaw),
        sinf(global_cam_pitch),
        cosf(global_cam_pitch) * cosf(global_cam_yaw)
    };

    global_camera_height_current = Vector3Lerp(
        global_camera_height_current,
        global_camera_height,
        GetFrameTime() * 10
    );

    float move_spd = Vector3Length(target->velocity);
    Vector3 base_viewmodel_pos = weapons[current_weapon].position;
    float bob_offset = sinf(camera_bob_phase) * camera_bob_amp * move_spd;

    // Set camera position just above player position (eye height)
    camera->position = Vector3Add(target->position, global_camera_height_current);

    float ls = 0.05; // lerp speed

    if (VIEWMODEL_POSITION_MODE == false && should_weapon_bob == true)
    {
        float delta = GetFrameTime();
        if (abs(move_spd) > 2)
        {
            camera->position.y += bob_offset;
            camera_bob_phase += delta * camera_bob_speed;
            viewmodel.position.z += bob_offset * weapon_bob;
        }
        else
        {
            viewmodel.position.x = Lerp(viewmodel.position.x, base_viewmodel_pos.x, ls);
            viewmodel.position.y = Lerp(viewmodel.position.y, base_viewmodel_pos.y, ls);
            viewmodel.position.z = Lerp(viewmodel.position.z, base_viewmodel_pos.z, ls);
            camera_bob_phase = 0.0f;
        }


        if (!global_player_onground)
        {
            bob_offset = 0.0;
            viewmodel.position.x = Lerp(viewmodel.position.x, base_viewmodel_pos.x, ls);
            viewmodel.position.y = Lerp(viewmodel.position.y, base_viewmodel_pos.y, ls);
            viewmodel.position.z = Lerp(viewmodel.position.z, base_viewmodel_pos.z, ls);
            camera_bob_phase = 0.0f;
        }
    }

    // Camera target is forward direction from eye position
    camera->target = Vector3Add(camera->position, forward);
    float desired_tilt = 0.0f;
    float strafe = 0.0f;

    if (should_camera_tilt)
    {
        // Keyboard
        if (IsKeyDown(BUTTON_MOVE_LEFT_KEY)) strafe = -1.0f;
        if (IsKeyDown(BUTTON_MOVE_RIGHT_KEY)) strafe =  1.0f;

        // Gamepad (left stick X axis)
        if (IsGamepadAvailable(GAMEPAD_P1)) {
            float axis = GetGamepadAxisMovement(GAMEPAD_P1, GAMEPAD_AXIS_LEFT_X);
            float deadzone = 0.2f;
            if (axis < -deadzone) strafe = -1.0f;
            else if (axis > deadzone) strafe = 1.0f;
        }

        if (strafe != 0.0f) {
            desired_tilt = Clamp(-strafe * camera_tilt_max, -camera_tilt_max, camera_tilt_max);
        } else {
            desired_tilt = 0.0f;
        }

        // Smoothly interpolate tilt for responsiveness
        camera_tilt = Lerp(camera_tilt, desired_tilt, GetFrameTime() * camera_tilt_speed);
    }

    // Apply tilt to camera's up vector (roll)
    camera->up = Vector3RotateByAxisAngle((Vector3){0.0f, 1.0f, 0.0f}, camera_get_forward(camera), camera_tilt);

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

void camera_update_fovy(Camera3D *camera)
{
    camera->fovy = 90.0f;
}