#include <draw_gui.h>
#include <stdio.h>
#include "raylib.h"
#include "raygui.h"
#include <global.h>

/*
draw_gui
-- any calls to render GUI or HUD elements should be called here
*/
void draw_gui()
{
    // Draw the FPS counter
    char fps_text[16];
    sprintf(fps_text, "%d", GetFPS());
    DrawText(fps_text, 10, 10, 20, BLACK);

    if (paused) draw_pausemenu();
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
        CloseWindow(); // Closes the window and exits the game
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