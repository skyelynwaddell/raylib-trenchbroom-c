#include "stdio.h"
#include "raylib.h"
#include "raymath.h"
#include "../headers/map.h"
#include "../headers/triangle.h"
#include "../headers/brushtopolygon.h"
#include "../headers/polygon.h"
#include <camera.h>

/*
draw
-- any calls to render -things- into the 3D world should be placed here
*/
void draw()
{
    ClearBackground(RAYWHITE);
    BeginMode3D(camera);
    map_draw();
}