#include "stdio.h"
#include "raylib.h"
#include "raymath.h"
#include "map.h"
#include "triangle.h"
#include "brushtopolygon.h"
#include "polygon.h"

/*
draw
-- any calls to render -things- into the 3D world should be placed here
*/
void draw()
{
    map_draw();
}