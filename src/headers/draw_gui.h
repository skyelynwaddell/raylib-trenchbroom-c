#ifndef DRAW_GUI_H
#define DRAW_GUI_H

#include <stdio.h>
#include "raylib.h"
#include "raygui.h"
#include <global.h>

void draw_gui();
void draw_pausemenu();
void menu_item_start();
void menu_item_next();
int get_pos_y(); // Returns the current Y position for the next GUI element

#endif // DRAW_GUI_H