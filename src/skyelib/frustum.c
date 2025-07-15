#include "frustum.h"

/*
frustum_get_from_camera
Gets what the camera can SEE currently on the screen aka the frustum
*/
Frustum frustum_get_from_camera(Camera3D cam)
{
    Matrix view = MatrixLookAt(cam.position, cam.target, cam.up);
    Matrix proj = MatrixPerspective(cam.fovy * DEG2RAD, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.01f, 1000.0f);
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
