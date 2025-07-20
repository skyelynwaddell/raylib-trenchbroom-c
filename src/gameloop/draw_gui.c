#include "gameloop.h"
#include "skyelib.h"
#include "global.h"

/*
draw_gui
-- any calls to render GUI or HUD elements should be called here
*/
void draw_gui()
{
    // Draw Crosshair
    DrawRectangle(
        SCREEN_WIDTH/2-(CROSSHAIR_SIZE/2),
        SCREEN_HEIGHT/2-(CROSSHAIR_SIZE/2),
        CROSSHAIR_SIZE,
        CROSSHAIR_SIZE,
        global_raycast_has_target ? RED : WHITE
        );

    console_draw();

    // Draw the FPS counter
    char fps_text[16];
    sprintf(fps_text, "%d", GetFPS());
    int text_width = MeasureText(fps_text, 20);
    int x = SCREEN_WIDTH - text_width - 10;     
    DrawText(fps_text, x, 5, 20, GetFPS() < 30 ? RED : WHITE);     

    if (global_paused) draw_pausemenu();

    //stats_draw();

}

/*
draw_pausemenu
-- This function draws the pause menu when the game is paused duh
*/
int btnIndex = 0; // Index for button positioning
int posX = 0;     // Starting X position for buttons
int posY = 100;   // Starting Y position for buttons
int textSize = 100;
int btnWidth = 300;
int btnHeight = 60;
int btnGap = 70;

void draw_pausemenu()
{
    menu_item_start();
    posX = GetScreenWidth() / 2 - btnWidth / 2;

    // Pause Menu Title
    GuiLabel((Rectangle){ posX, get_pos_y(), btnWidth, btnHeight }, "Pause Menu");

    // Resume Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(), btnWidth, btnHeight }, "Resume"))
    {
        pause_toggle(); // Toggle pause state
    }

    // Save Game Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(), btnWidth, btnHeight }, "Save Game"))
    {
        // TODO : Implement save game functionality
    }

    // Load Game Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(), btnWidth, btnHeight }, "Load Game"))
    {
        // TODO : Implement load game functionality
    }

    // Exit Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(), btnWidth, btnHeight }, "Exit"))
    {
        global_quit_game = true;
    }
}


/* 
menu_item_start
Must be called before adding any menu items to reset the button index.
*/
void menu_item_start() { btnIndex = 0; } // Reset the button index to start from the top


/*
new_menu_item
Call this function before adding a new menu item to increment the button index.
*/
void menu_item_next() { btnIndex++; }


/*
get_pos_y
This function calculates the Y position for the current menu item based on the index and gap.
*/
int get_pos_y() { return posY + (btnGap * btnIndex); }