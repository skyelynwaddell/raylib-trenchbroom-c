#include "skyelib.h"
#include "global.h"

/*
GuiwindowFloating
Draws a window that floats, and draws whatever is defined in the void "content" function
*/
void GuiWindowFloating(Vector2 *position, Vector2 *size, bool *minimized, bool *moving, bool *resizing, void (*draw_content)(Vector2, Vector2), Vector2 content_size, Vector2 *scroll, const char* title) {

    int close_title_size_delta_half = (raygui_windowbox_statusbar_height - raygui_window_closebutton_size) / 2;

    // window movement and resize input and collision check
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !*moving && !*resizing) {
        Vector2 mouse_position = GetMousePosition();

        Rectangle title_collision_rect = { position->x, position->y, size->x - (raygui_window_closebutton_size + close_title_size_delta_half), raygui_windowbox_statusbar_height };
        Rectangle resize_collision_rect = { position->x + size->x - 20, position->y + size->y - 20, 20, 20 };

        if(CheckCollisionPointRec(mouse_position, title_collision_rect)) {
            *moving = true;
        } else if(!*minimized && CheckCollisionPointRec(mouse_position, resize_collision_rect)) {
            //*resizing = true;
        }
    }

    // window movement and resize update
    if(*moving) {
        Vector2 mouse_delta = GetMouseDelta();
        position->x += mouse_delta.x;
        position->y += mouse_delta.y;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            *moving = false;
        }

    } else if(*resizing) {
        Vector2 mouse = GetMousePosition();
        if (mouse.x > position->x)
            size->x = mouse.x - position->x;
        if (mouse.y > position->y)
            size->y = mouse.y - position->y;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            *resizing = false;
        }
    }

    int gap = 20;
    // clamp window position keep it inside the application area
    if(position->x < 0) position->x = 0;
    else if(position->x > GetGameScreenWidth() - size->x) position->x = GetGameScreenWidth() - size->x;
    if(position->y < 0) position->y = 0;
    else if(position->y > GetGameScreenHeight() - size->y) position->y = GetGameScreenHeight() - size->y;

    // clamp window size to an arbitrary minimum value and the window size as the maximum
    if(size->x < 100) size->x = 100;
    else if(size->x > GetGameScreenWidth()) size->x = GetGameScreenWidth();
    if(size->y < 100) size->y = 100;
    else if(size->y > GetGameScreenHeight()) size->y = GetGameScreenHeight();

    // window and content drawing with scissor and scroll area
    if(*minimized) {
        gui_state = GUI_STATE_DEFAULT;
        *minimized = false;

    } else {
        font_set_size(9*gui_scale);
        *minimized = GuiWindowBox((Rectangle) { position->x, position->y, size->x, size->y }, title);

        font_set_size(fontsize*gui_scale);

        // scissor and draw content within a scroll panel
        if(draw_content != NULL) {
            Rectangle scissor = { 0 };
            GuiScrollPanel((Rectangle) { position->x, position->y + raygui_windowbox_statusbar_height, size->x, size->y - raygui_windowbox_statusbar_height },
                                         NULL,
                                         (Rectangle) { position->x, position->y, content_size.x, content_size.y },
                                         scroll,
                                         &scissor);

            bool require_scissor = size->x < content_size.x || size->y < content_size.y;

            if(require_scissor) {
                BeginScissorMode(scissor.x, scissor.y, scissor.width, scissor.height);
            }

            draw_content(*position, *scroll);

            if(require_scissor) {
                EndScissorMode();
            }
        }

        // draw the resize button/icon
        //GuiDrawIcon(71, position->x + size->x - 20, position->y + size->y - 20, 1, WHITE);
    }
}

