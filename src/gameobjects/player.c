#include "player.h"
#include "skyelib.h"

Player player;

/*
player_init
Called when the player object first enters the world
*/
void player_init()
{
    #ifdef DEBUG
        printf("### Player Object Created ###\n\n");
    #endif

    // spawn position
    player.gameobject.position = global_player_spawn;

    // create player collision box
    float h = PLAYER_HEIGHT; // units tall
    float w = PLAYER_WIDTH;  // units thick/wide
    collisionbox_set(
        &player.gameobject.collision_box,
        player.gameobject.position,
        (Vector3){ w, h, w },
        1
    );

    // set player stats
    player.health = PLAYER_HEALTH;
    player.jump_height = PLAYER_JUMPHEIGHT;
    player.gameobject.speed = PLAYER_SPEED;
    player.gameobject.gravity = GRAVITY_DEFAULT;

    global_camera_height_current = global_camera_height;
};

static float shoot_cooldown = 0;
static float shoot_cooldown_max = 0.3;


/*
player_shoot
Call this to invoke the player shooting
*/
static void player_shoot()
{
    global_player_shooting = true;
    viewmodel.model.current_frame = 0;
    viewmodel.model.current_anim = ANIM_PISTOL_SHOOT;
    shoot_cooldown = shoot_cooldown_max;
}


/*
player_handle_shoot
Handles if the player can shoot, and the button presses
*/
static void player_handle_shoot(float delta)
{
    if (shoot_cooldown > 0) 
    {
        if (viewmodel.model.current_anim_finished)
        shoot_cooldown -= delta;
        shoot_cooldown = Clamp(shoot_cooldown,0,shoot_cooldown_max);

        if (shoot_cooldown <= 0.1)
            viewmodel.model.current_anim = ANIM_PISTOL_IDLE;
        return;
    }

    if (IsMouseButtonDown(BUTTON_SHOOT_KEY) || 
    IsGamepadButtonDown(GAMEPAD_P1, BUTTON_SHOOT_PAD))
        player_shoot();
}


/*
player_update
Called every Tick
*/
void player_update()
{
    check_collisions(&player, true, COLLISION_MASK_ALL);

    if (global_paused || global_game_loading || global_console_open) 
    {
        player.gameobject.velocity = Vector3Zero();
        return;
    }

    float delta = GetFrameTime();

    player_handle_crouch();
    player_movement();
    apply_gravity(&player);
    player_handle_jump();
    camera_follow_player(&camera, &player);
    player_handle_shoot(delta);
}


/*
player_draw
Any calls to draw 3d objects on the player can be
ie. collision box shape etc
For drawing weapons, hands, arms etc. see Viewmodel to draw instead
This is for actual player model etc not the arms or view model weapon.
*/
void player_draw()
{
    #ifdef DEBUG
    DrawBoundingBox(player.gameobject.collision_box.bounding_box, RED);
    #endif
}


/*
player_handle_crouch
Handles when the player presses the Crouch key,
and toggles the player crouching moving up/down
*/
void player_handle_crouch()
{
    if (IsKeyPressed(BUTTON_CROUCH_KEY) || IsGamepadButtonPressed(GAMEPAD_P1, BUTTON_CROUCH_PAD))
    {
        if (global_player_crouching == false)
        {
            global_player_crouching = true;
            global_camera_height = CAMERA_HEIGHT_CROUCH;
        }
        else
        {
            global_player_crouching = false;
            global_camera_height = CAMERA_HEIGHT_DEFAULT;
        }
    }  
}



/*
player_movement
Handles player directional input, movement, acceleration, and friction
*/
void player_movement()
{   
    float input_x, input_z;

    // keyboard movement
    input_x = (IsKeyDown(BUTTON_MOVE_LEFT_KEY) - IsKeyDown(BUTTON_MOVE_RIGHT_KEY));
    input_z = (IsKeyDown(BUTTON_MOVE_FORWARD_KEY) - IsKeyDown(BUTTON_MOVE_BACKWARD_KEY));

    // gamepad movement
    if (IsGamepadAvailable(GAMEPAD_P1) && (input_x == 0 && input_z == 0))
    {
        float lx = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        float ly = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

        float deadzone = 0.2f;

        if (fabsf(lx) < deadzone) lx = 0.0f;
        if (fabsf(ly) < deadzone) ly = 0.0f;

        input_x = -lx;
        input_z = -ly;
    }

    float input_len = sqrtf(input_x * input_x + input_z * input_z);
    if (input_len > 0.0f) {
        input_x /= input_len;
        input_z /= input_len;
    }

    Vector3 forward = { sinf(global_cam_yaw), 0.0f, cosf(global_cam_yaw)  };
    Vector3 right   = { cosf(global_cam_yaw), 0.0f, -sinf(global_cam_yaw) };

    Vector3 move_dir = {
        forward.x * input_z + right.x * input_x,
        0.0f,
        forward.z * input_z + right.z * input_x
    };

    float accel = 5.0f;
    float fric = 10.0f;
    float air_fric = 1.0f;
    float dt = GetFrameTime();

    if (input_len > 0.0f)
    {
        Vector3 desired = Vector3Scale(move_dir, player.gameobject.speed * input_len);
        Vector3 delta = Vector3Subtract(desired, (Vector3){ player.gameobject.velocity.x, 0.0f, player.gameobject.velocity.z });
        Vector3 accel_step = Vector3Scale(delta, accel * dt);
        player.gameobject.velocity.x += accel_step.x;
        player.gameobject.velocity.z += accel_step.z;
    }
    else
    {
        // apply friction only when no input
        float current_fric = global_player_onground ? fric : air_fric;
        player.gameobject.velocity.x = Lerp(player.gameobject.velocity.x, 0.0f, current_fric * dt);
        player.gameobject.velocity.z = Lerp(player.gameobject.velocity.z, 0.0f, current_fric * dt);
    }
}


/*
player_jump
Call this to invoke the player jumping into the air
*/
void player_jump()
{
    player.gameobject.velocity.y = player.jump_height;
    global_player_onground = false;
}


/*
player_handle_jump
Called to handle when / if the player CAN jump
*/
void player_handle_jump()
{
    // jump
    if (((GetMouseWheelMove() < 0) || IsKeyPressed(BUTTON_JUMP_KEY) || (IsGamepadButtonPressed(GAMEPAD_P1, BUTTON_JUMP_PAD))) && global_player_onground)
        player_jump();
}