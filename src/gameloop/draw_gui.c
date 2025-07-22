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
        GAME_SCREEN_WIDTH/2-(CROSSHAIR_SIZE/2),
        GAME_SCREEN_HEIGHT/2-(CROSSHAIR_SIZE/2),
        CROSSHAIR_SIZE,
        CROSSHAIR_SIZE,
        global_raycast_has_target ? RED : WHITE
        );

    console_draw();

    // Draw the FPS counter
    char fps_text[16];
    sprintf(fps_text, "%d", GetFPS());
    int text_width = MeasureText(fps_text, 20);
    int x = GAME_SCREEN_WIDTH - text_width - 10;     
    DrawText(fps_text, x, 5, 20, GetFPS() < 30 ? RED : WHITE);     

    if (global_paused) draw_pausemenu();
    //stats_draw();

}

int btnIndex = 0; // Index for button positioning
int posX = 0;     // Starting X position for buttons
int posY = 0;     // Starting Y position for buttons
int btnWidth = 130;   // Use a more reasonable base width
int btnHeight = 26;   // Use a more reasonable base height
int btnGap = 33;      // Use a more reasonable base gap

float scale = 1.0f;   // Store scale globally for get_pos_y()

void draw_pausemenu()
{
    float scale = 2.0;//Min((float)GetScreenWidth()/GAME_SCREEN_WIDTH, (float)GetScreenHeight()/GAME_SCREEN_HEIGHT) * gui_scale;
    //scale = Clamp(rawScale, 1.0, 1.4f); // Clamp scale between 0.75 and 1.5

    menu_item_start();

    posY = (int)(10 * scale);
    posX = (int)(10 * scale);
    btnGap = (int)(27 * scale);

    int scaledBtnWidth = (int)(btnWidth * scale);
    int scaledBtnHeight = (int)(btnHeight * scale);
    int scaledBtnGap = (int)(btnGap * scale);

    // Pause Menu Title
    DrawText("Game Paused", posX, posY, (int)(20 * scale), WHITE);

    font_set_size((int)(10 * scale));

    // Resume Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(scaledBtnGap), scaledBtnWidth, scaledBtnHeight }, "Resume"))
    {
        pause_toggle();
    }

    // Save Game Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(scaledBtnGap), scaledBtnWidth, scaledBtnHeight }, "Save Game"))
    {
        // TODO : Implement save game functionality
    }

    // Load Game Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(scaledBtnGap), scaledBtnWidth, scaledBtnHeight }, "Load Game"))
    {
        // TODO : Implement load game functionality
    }

    // Exit Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(scaledBtnGap), scaledBtnWidth, scaledBtnHeight }, "Exit"))
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