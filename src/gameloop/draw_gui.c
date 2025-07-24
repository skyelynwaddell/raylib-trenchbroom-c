#include "gameloop.h"
#include "skyelib.h"
#include "global.h"
#include "floatingwindow.h"

static void draw_viewmodel_editmode()
{
    if (VIEWMODEL_POSITION_MODE == false) return;

    int y = 50;
    int g = 20;
    DrawText("Position Viewmodel Mode",10,y,20,WHITE); y+=g;
    DrawText("Controls:",10,y,20,WHITE); y+=g;
    DrawText("Left/Right : X",10,y,20,WHITE); y+=g;
    DrawText("Up/Down   : Y",10,y,20,WHITE); y+=g;
    DrawText("[ / ]         : Z",10,y,20,WHITE); y+=g;
    DrawText(", / .         : Rotation",10,y,20,WHITE); y+=g;
    DrawText(TextFormat("Viewmodel Pos:\nX:%.2f \nY:%.2f \nZ:%.2f \nR:%d \n", viewmodel.position.x, viewmodel.position.y, viewmodel.position.z, (int)viewmodel.rotation),10,y,20,WHITE); y+=g;
}


/*
draw_gui
-- any calls to render GUI or HUD elements should be called here
*/
void draw_gui()
{

    draw_viewmodel_editmode();

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
    if (show_fps)
    {
        int font_size = 11;
        char fps_text[16];
        sprintf(fps_text, "%d", GetFPS());
        int text_width = MeasureText(fps_text, font_size*gui_scale);
        int x = GAME_SCREEN_WIDTH - text_width - 10;     
        DrawText(fps_text, x, 5, font_size*gui_scale, GetFPS() <= 25 ? RED : WHITE);    
    } 

    if (global_paused) draw_pausemenu();

    if (gui_state == GUI_STATE_OPTIONS)
        GuiWindowFloating(&window_position, &window_size, &window_minimized, &window_moving, &window_resizing, &draw_options_menu, (Vector2) { 400, 400 }, &window_scroll, "#142# Options");
    //stats_draw();

}

int btnIndex = 0; // Index for button positioning
int posX = 0;     // Starting X position for buttons
int posY = 0;     // Starting Y position for buttons
int btnWidth = 130;   // Use a more reasonable base width
int btnHeight = 26;   // Use a more reasonable base height
int btnGap = 33;      // Use a more reasonable base gap

void draw_pausemenu()
{
    float scale = gui_scale;//Min((float)GetScreenWidth()/GAME_SCREEN_WIDTH, (float)GetScreenHeight()/GAME_SCREEN_HEIGHT) * gui_scale;
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

    Rectangle window_rect = {
        window_position.x,
        window_position.y,
        window_size.x,
        window_size.y
    };

    Vector2 mouse = GetMousePosition();

    bool mouse_over_window = false;
    if (gui_state != GUI_STATE_DEFAULT)
        mouse_over_window = CheckCollisionPointRec(mouse, window_rect);

    // Resume Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(scaledBtnGap), scaledBtnWidth, scaledBtnHeight }, "Resume"))
    {
        if (mouse_over_window == false)
        {
            pause_toggle();
            gui_state = GUI_STATE_DEFAULT;
        }
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

    // Options Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(scaledBtnGap), scaledBtnWidth, scaledBtnHeight }, "Options"))
    {
        if (mouse_over_window == false)
        {
            gui_state = gui_state == GUI_STATE_DEFAULT ? GUI_STATE_OPTIONS : GUI_STATE_DEFAULT;
            options_window_init();
        }
    }

    // Exit Button
    menu_item_next();
    if (GuiButton((Rectangle){ posX, get_pos_y(scaledBtnGap), scaledBtnWidth, scaledBtnHeight }, "Exit"))
    {
        if (mouse_over_window == false)
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