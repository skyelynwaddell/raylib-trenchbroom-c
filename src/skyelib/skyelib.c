#define RAYGUI_IMPLEMENTATION
#include "skyelib.h"

void font_set_size(int size){ GuiSetStyle(DEFAULT, TEXT_SIZE, size); }

int GAME_SCREEN_WIDTH = 1280;
int GAME_SCREEN_HEIGHT = 720;

int GetGameScreenWidth(){ return GAME_SCREEN_WIDTH; }
int GetGameScreenHeight(){ return GAME_SCREEN_HEIGHT; }

float gui_scale = 1.5;

Vector2 SCREEN_CENTER() 
{ 
    return (Vector2){ 
        GetScreenWidth() / 2.0f, 
        GetScreenHeight() / 2.0f
    };
}
Ray CENTER_RAY(Camera cam) { return GetScreenToWorldRay(SCREEN_CENTER(), cam); }

/*
BeginModeViewModel()
Starts the 3D mode for the view model rendering.
*/
void BeginModeViewModel()
{
    rlDrawRenderBatchActive();
    glClear(GL_DEPTH_BUFFER_BIT);
    BeginMode3D(viewmodel.camera);
}


/*
EndModeViewModel()
Ends the 3D mode for the view model rendering.
*/
void EndModeViewModel()
{
    EndMode3D();
}


/*
to_delta
Returns value multiplied by Delta Time
*/
float to_delta(float value)
{
    return value * GetFrameTime();
}


/*
frustum_get_from_camera
Gets what the camera can SEE currently on the screen aka the frustum
*/
Frustum frustum_get_from_camera(Camera3D cam)
{
    Matrix view = MatrixLookAt(cam.position, cam.target, cam.up);

    #ifdef DEBUG
        float _fov = 0.9f; // reduce FOV by 10% in DEBUG to show Occlussion Culling is working
    #else
        float _fov = 1.0f;
    #endif
    
    float fov = cam.fovy * _fov;
    Matrix proj = MatrixPerspective(fov * DEG2RAD, (float)GAME_SCREEN_WIDTH / GAME_SCREEN_HEIGHT, 0.01f, 1000.0f);

    Matrix vp = MatrixMultiply(view, proj);

    Frustum frustum;

    frustum.left   = (Vector4){ vp.m3 + vp.m0,  vp.m7 + vp.m4,  vp.m11 + vp.m8,  vp.m15 + vp.m12 };
    frustum.right  = (Vector4){ vp.m3 - vp.m0,  vp.m7 - vp.m4,  vp.m11 - vp.m8,  vp.m15 - vp.m12 };
    frustum.bottom = (Vector4){ vp.m3 + vp.m1,  vp.m7 + vp.m5,  vp.m11 + vp.m9,  vp.m15 + vp.m13 };
    frustum.top    = (Vector4){ vp.m3 - vp.m1,  vp.m7 - vp.m5,  vp.m11 - vp.m9,  vp.m15 - vp.m13 };
    frustum.near   = (Vector4){ vp.m3 + vp.m2,  vp.m7 + vp.m6,  vp.m11 + vp.m10, vp.m15 + vp.m14 };
    frustum.far    = (Vector4){ vp.m3 - vp.m2,  vp.m7 - vp.m6,  vp.m11 - vp.m10, vp.m15 - vp.m14 };

    frustum.left   = Vector4Normalize(frustum.left);
    frustum.right  = Vector4Normalize(frustum.right);
    frustum.bottom = Vector4Normalize(frustum.bottom);
    frustum.top    = Vector4Normalize(frustum.top);
    frustum.near   = Vector4Normalize(frustum.near);
    frustum.far    = Vector4Normalize(frustum.far);

    return frustum;
}


/*
frustum_check_boundingbox
Returns TRUE if a BoundingBox is inside the Frustum
*/
int frustum_check_boundingbox(BoundingBox box, Frustum f)
{
    Vector3 corners[8] = {
        {box.min.x, box.min.y, box.min.z},
        {box.max.x, box.min.y, box.min.z},
        {box.min.x, box.max.y, box.min.z},
        {box.max.x, box.max.y, box.min.z},
        {box.min.x, box.min.y, box.max.z},
        {box.max.x, box.min.y, box.max.z},
        {box.min.x, box.max.y, box.max.z},
        {box.max.x, box.max.y, box.max.z},
    };

    Vector4 planes[6] = { f.left, f.right, f.top, f.bottom, f.near, f.far };

    for (int i = 0; i < 6; i++)
    {
        int outside = 0;
        for (int j = 0; j < 8; j++)
        {
            Vector3 p = corners[j];
            if ((planes[i].x * p.x + planes[i].y * p.y + planes[i].z * p.z + planes[i].w) < 0.0f)
                outside++;
        }

        // All 8 corners are outside 1 plane = not visible
        if (outside == 8)
            return false;
    }

    return true; // At least partially inside
}


/*
frustum_check_sphere
Returns TRUE if a sphere is inside or intersecting the frustum
*/
int frustum_check_sphere(Vector3 center, float radius, Frustum f)
{
    Vector4 planes[6] = { f.left, f.right, f.top, f.bottom, f.near, f.far };

    for (int i = 0; i < 6; i++)
    {
        float distance = planes[i].x * center.x + 
                         planes[i].y * center.y + 
                         planes[i].z * center.z + 
                         planes[i].w;

        // Completely outside this plane
        if (distance < -radius)
            return false;
    }

    return true; // Inside or intersecting
}


/*
string_equals
string[char* string] - The first string to be compared
string[char* string_to_compare_to] - the second string that is compared to the first string
-- compares two strings to see if they match
*/
int string_equals(char* string, char* string_to_compare_to)
{
    if (strcmp(string, string_to_compare_to) == 0) return 1;
    return 0;
}


/*
convert_trenchbroom_to_raylib_axis
-- raylib and trenchbroom dont use the same xyz axis, so we have to convert this here
-- so our stuff isnt sideways :P
*/
Vector3 trench_to_raylib_origin(Vector3 v)
{
    float s = 0.1; // scale
    Vector3 newpos = (Vector3) { v.x*s, v.y*s, -v.z*s };
    return newpos;
}


/*
rotate_vector_around_axis
-- takes in a vector3, a vector3 axis, and the angle degrees
-- and rotates the origin vec around the second vector3
*/
Vector3 rotate_vector_around_axis(Vector3 vec, Vector3 axis, float angle)
{
    axis = Vector3Normalize(axis);
    float cos_theta = cosf(angle);
    float sin_theta = sinf(angle);

    return Vector3Add(
        Vector3Add(
            Vector3Scale(vec, cos_theta),
            Vector3Scale(Vector3CrossProduct(axis, vec), sin_theta)
        ),
        Vector3Scale(axis, Vector3DotProduct(axis, vec) * (1 - cos_theta))
    );
}


/*
Vector3DoubleCrossProduct
// Calculate two vectors cross product
*/
Vector3Double Vector3DoubleCrossProduct(Vector3Double v1, Vector3Double v2)
{
    Vector3Double result = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
    return result;
}


/*
Vector3DoubleScale
// Multiply vector by scalar
*/
Vector3Double Vector3DoubleScale(Vector3Double v, float scalar)
{
    Vector3Double result = { v.x*scalar, v.y*scalar, v.z*scalar };
    return result;
}


// Calculate two vectors dot product
double Vector3DoubleDotProduct(Vector3Double v1, Vector3Double v2)
{
    float result = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
    return result;
}


// Add two vectors
Vector3Double Vector3DoubleAdd(Vector3Double v1, Vector3Double v2)
{
    Vector3Double result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    return result;
}


// Subtract two vectors
Vector3Double Vector3DoubleSubtract(Vector3Double v1, Vector3Double v2)
{
    Vector3Double result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
    return result;
}


// Normalize provided vector
Vector3Double Vector3DoubleNormalize(Vector3Double v)
{
    Vector3Double result = v;

    float length = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length != 0.0f)
    {
        float ilength = 1.0f/length;

        result.x *= ilength;
        result.y *= ilength;
        result.z *= ilength;
    }

    return result;
}


/*
rlGetVersionString
Returns the current OpenGL version as formatted
string :)
*/
const char *rlGetVersionString(void)
{
    switch (rlGetVersion())
    {
        case RL_OPENGL_11: return "OpenGL: v1.1";
        case RL_OPENGL_21: return "OpenGL: v2.1";
        case RL_OPENGL_33: return "OpenGL: v3.3";
        case RL_OPENGL_43: return "OpenGL: v4.3";
        case RL_OPENGL_ES_20: return "OpenGL ES: v2.0";
        case RL_OPENGL_ES_30: return "OpenGL ES: v3.0";
        default: return "OpenGL";
    }
}
