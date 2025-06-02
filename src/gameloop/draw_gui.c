#include <stdio.h>
#include "raylib.h"

void draw_gui()
{
    char fps_text[16];
    sprintf(fps_text, "%d", GetFPS());
    DrawText(fps_text, 10, 10, 20, BLACK);
}