#ifndef SKYELIB_H
#define SKYELIB_H

#define PLATFORM_DESKTOP
#define DEBUG

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

// game settings
#define GAME_TITLE "skyesrc"
#define FPS 2000 // max fps

#define MAX_VERTICES_PER_FACE 128
#define BRUSH_FACE_COUNT 64         // a brush can have up to 64 faces - must be convex
#define MAX_TEXTURES 10000
#define CAMERA_HEIGHT (Vector3){0.0f, 3.5f, 0.0f} // adjust for player height
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// brush & map geometry defs
#define MAX_VERTICES_PER_FACE 128   // a brushface can have up to 128 vertices
#define MAX_COLUMNS 20
#define MAX_POLY_VERTS 64
#define MAX_LINE 1024
#define MAX_BRUSHES 10000
#define MAX_POLYGONS 10000

// level settings
#define MAX_ENTITIES 1000 // Maximum entities can be in a room
#define MAX_LIGHTS 255 // Maximum lightobjects can be in a room
#define MAX_DARK 0.2 // How dark the room can get without lighting (0 = BLACK)

// controls
// --- Movement ---
#define GAMEPAD_P1 0

// --- Movement ---
#define BUTTON_MOVE_FORWARD_KEY      KEY_W
#define BUTTON_MOVE_FORWARD_PAD      GAMEPAD_BUTTON_LEFT_FACE_UP

#define BUTTON_MOVE_BACKWARD_KEY     KEY_S
#define BUTTON_MOVE_BACKWARD_PAD     GAMEPAD_BUTTON_LEFT_FACE_DOWN

#define BUTTON_MOVE_LEFT_KEY         KEY_A
#define BUTTON_MOVE_LEFT_PAD         GAMEPAD_BUTTON_LEFT_FACE_LEFT

#define BUTTON_MOVE_RIGHT_KEY        KEY_D
#define BUTTON_MOVE_RIGHT_PAD        GAMEPAD_BUTTON_LEFT_FACE_RIGHT

// --- Actions ---
#define BUTTON_JUMP_KEY              KEY_SPACE
#define BUTTON_JUMP_PAD              GAMEPAD_BUTTON_RIGHT_FACE_DOWN  // X / A

#define BUTTON_CROUCH_KEY            KEY_LEFT_CONTROL
#define BUTTON_CROUCH_PAD            GAMEPAD_BUTTON_RIGHT_FACE_LEFT  // Y / X

#define BUTTON_SPRINT_KEY            KEY_LEFT_SHIFT
#define BUTTON_SPRINT_PAD            GAMEPAD_BUTTON_RIGHT_FACE_UP    // B / Circle

// --- Combat ---
#define BUTTON_SHOOT_KEY             MOUSE_LEFT_BUTTON
#define BUTTON_SHOOT_PAD             GAMEPAD_BUTTON_RIGHT_TRIGGER

#define BUTTON_AIM_KEY               MOUSE_RIGHT_BUTTON
#define BUTTON_AIM_PAD               GAMEPAD_BUTTON_LEFT_TRIGGER

#define BUTTON_RELOAD_KEY            KEY_R
#define BUTTON_RELOAD_PAD            GAMEPAD_BUTTON_RIGHT_FACE_RIGHT // B / Circle

#define BUTTON_MELEE_KEY             KEY_C
#define BUTTON_MELEE_PAD             GAMEPAD_BUTTON_LEFT_FACE_UP     // optional

// --- Weapons ---
#define BUTTON_SWITCH_WEAPON_KEY     KEY_Q
#define BUTTON_SWITCH_WEAPON_PAD     GAMEPAD_BUTTON_RIGHT_FACE_UP    // Y / Triangle

#define BUTTON_NEXT_WEAPON_KEY       KEY_E
#define BUTTON_NEXT_WEAPON_PAD       GAMEPAD_BUTTON_LEFT_FACE_RIGHT

#define BUTTON_PREV_WEAPON_KEY       KEY_Q
#define BUTTON_PREV_WEAPON_PAD       GAMEPAD_BUTTON_LEFT_FACE_LEFT

#define BUTTON_FLASHLIGHT_KEY        KEY_F
#define BUTTON_FLASHLIGHT_PAD        GAMEPAD_AXIS_RIGHT_TRIGGER // You may want to use a value check for this one

// --- UI / Menus ---
#define BUTTON_PAUSE_KEY             KEY_ESCAPE
#define BUTTON_PAUSE_PAD             GAMEPAD_BUTTON_MIDDLE_RIGHT     // Start

#define BUTTON_INTERACT_KEY          KEY_E
#define BUTTON_INTERACT_PAD          GAMEPAD_BUTTON_RIGHT_FACE_LEFT  // X / Square

#define BUTTON_USE_KEY               KEY_E
#define BUTTON_USE_PAD               GAMEPAD_BUTTON_RIGHT_FACE_LEFT  // same as interact

// raygui & styles/themes
#define STYLE_PATH "styles/"
#define STYLE_AMBER STYLE_PATH "style_amber.rgs"
#define STYLE_ASHES STYLE_PATH "style_ashes.rgs"
#define STYLE_BLUISH STYLE_PATH "style_bluish.rgs"
#define STYLE_CANDY STYLE_PATH "style_candy.rgs"
#define STYLE_CHERRY STYLE_PATH "style_cherry.rgs"
#define STYLE_CYBER STYLE_PATH "style_cyber.rgs"
#define STYLE_DARK STYLE_PATH "style_dark.rgs"
#define STYLE_ENFETE STYLE_PATH "style_enfete.rgs"
#define STYLE_GENESIS STYLE_PATH "style_genesis.rgs"
#define STYLE_JUNGLE STYLE_PATH "style_jungle.rgs"
#define STYLE_LAVANDA STYLE_PATH "style_lavanda.rgs"
#define STYLE_RLTECH STYLE_PATH "style_rltech.rgs"
#define STYLE_SUNNY STYLE_PATH "style_sunny.rgs"
#define STYLE_TERMINAL STYLE_PATH "style_terminal.rgs"

#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include <stdio.h>
#include "string.h"
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "float.h"

extern Camera camera;
extern int camera_mode;
extern int camera_move_spd;
extern float global_cam_yaw;  // left/right
extern float global_cam_pitch; // up/down

typedef struct Triangle {
    Vector3 a,b,c;
} Triangle;

typedef struct Polygon {
    Vector3 vertices[MAX_VERTICES_PER_FACE];
    int vertex_count;
} Polygon;

typedef struct Plane {
    Vector3 normal; // plane normal (a,b,c)
    float distance; // plane distance from origin
} Plane;

typedef struct CollisionPolygon {
    Triangle *triangles;
    int count;
} CollisionPolygon;

typedef struct BrushFace {
    // Defines the position, size, and directions of the infinite plane
    Vector3 pos_1;    // [ x y z ] Origin Point of the plane --- The anchor point / position of the plane
    Vector3 pos_2;    // [ x y z ] with pos_1 defines the first infinite direction of the plane
    Vector3 pos_3;    // [ x y z ] with pos_1 defines the second infinite direction of the plane

    // with all the infinite planes in a brush we can clip all the geometry wherever 
    // any of the infinite planes intersect with eachother to form the convex polygon.
    
    // texture data for a brush face
    char texture[64]; // texture string name (not including filetype)

    Vector4 uv_s; // [ Ux Uy Uz Uoffset ]
    Vector4 uv_t; // [ Vx Vy Vz Voffset ]

    int uv_rotation; // texture rotation degrees
    int u_scale;     // horizontal texture scale
    int v_scale;     // vertical texture scale

} BrushFace;

typedef struct Brush {
    int brush_face_count;
    BrushFace brush_faces[BRUSH_FACE_COUNT];
    Polygon polys[BRUSH_FACE_COUNT];
} Brush;

typedef struct Frustum {
    Vector4 left, right, top, bottom, near, far;
} Frustum;

typedef struct Geometry {
    Model model;
    BoundingBox bounds;
    CollisionPolygon collision;
    Vector3 position;
} Geometry;

typedef struct CollisionBox {
    Vector3 position;
    Vector3 size;
    int scale;
    BoundingBox bounding_box;
} CollisionBox;

typedef struct GameObject {
    Vector3 position;
    Vector3 velocity;
    CollisionBox collision_box; // TODO : Make a Collision 
    float speed;
    float gravity;
} GameObject;

typedef struct Vector3Double {
    double x;
    double y;
    double z;
} Vector3Double;

typedef struct Entity {
    char classname[64];
    Vector3 origin;

    // light properties
    Color color;
    float brightness;
    float radius;
} Entity;

typedef struct TextureCacheEntry {
    char name[64];
    Texture2D texture;
} TextureCacheEntry;


// Texture Manager
extern int texture_cache_count;
extern TextureCacheEntry texture_cache[MAX_TEXTURES];
extern Texture2D default_texture;
Texture2D texture_get_cached(char *texture_name);
Texture2D texture_get_default();

// GameObject
int place_meeting_solid(GameObject *object);
int CheckCollisionBoxesExt(BoundingBox, CollisionPolygon shape);
int check_AABB_triangle_SAT(BoundingBox box, Triangle tri);
void apply_gravity(GameObject *obj);
void check_collisions(GameObject *obj, int is_player);

// Camera
void camera_init();
void camera_follow_player(Camera3D *camera, GameObject *target);
Vector3 camera_get_forward(Camera *camera);
Vector3 camera_get_up(Camera *camera);
Vector3 camera_get_right(Camera *camera);
void camera_yaw(Camera *camera, float angle, bool rotateAroundTarget);
void camera_pitch(Camera *camera, float angle, bool lockView, bool rotateAroundTarget, bool rotateUp);
void camera_roll(Camera *camera, float angle);

// Polygon
bool polygon_get_intersection(
    Vector3 n1,
    Vector3 n2,
    Vector3 n3,
    double d1,
    double d2,
    double d3,
    Vector3* out
);
void polygon_generate_from_brush(Brush *brush);
void polygon_sort_vertices(Polygon *poly, Vector3 normal);
void polygon_normalize_uv(Vector2 *uvs, int count);
bool polygon_has_vertex(Polygon *poly, Vector3 v);
Vector2 polygon_project_to_uv_standard(Vector3 point, BrushFace *face);
Vector2 polygon_project_to_uv_valve220(Vector3 point, BrushFace *face);

// Collision Box
void collisionbox_set(CollisionBox *box, Vector3 _position, Vector3 _size, int _scale);
void collisionbox_update_aabb(CollisionBox *box);
void collisionbox_set_position(CollisionBox *box, Vector3 _position);

// Brushface
void brushface_print(BrushFace b, int face_index);
Plane brushface_to_plane(BrushFace face);

// Frustum
Frustum frustum_get_from_camera(Camera3D cam);
int frustum_check_boundingbox(BoundingBox box, Frustum f);

// Vector3Double
Vector3Double Vector3DoubleCrossProduct(Vector3Double v1, Vector3Double v2);
Vector3Double Vector3DoubleScale(Vector3Double v, float scalar);
double Vector3DoubleDotProduct(Vector3Double v1, Vector3Double v2);
Vector3Double Vector3DoubleAdd(Vector3Double v1, Vector3Double v2);
Vector3Double Vector3DoubleSubtract(Vector3Double v1, Vector3Double v2);
Vector3Double Vector3DoubleNormalize(Vector3Double v);

// Utility Functions
Vector3 rotate_vector_around_axis(Vector3 vec, Vector3 axis, float angle);
Vector3 trench_to_raylib_origin(Vector3 v);
int string_equals(char* string, char* string_to_compare_to);
float to_delta(float value);

#endif // SKYELIB_H