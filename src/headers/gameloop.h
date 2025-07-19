#ifndef GAMELOOP_H
#define GAMELOOP_H
#include "skyelib.h"


int gameloop();
void init();
void input();
void update();
void draw();
void draw_gui();
void draw_viewmodel();
void clean_up();

void draw_pausemenu();
void menu_item_start();
void menu_item_next();
int get_pos_y(); // Returns the current Y position for the next GUI element
void handle_pause(); // Pause / Unpause the game when the user presses the ESC key

#endif // GAMELOOP_H