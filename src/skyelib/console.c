#include "skyelib.h"
#include "global.h"

#define MAX_CONSOLE_LINES 128
#define MAX_LINE_LENGTH 1024

static char console_lines[MAX_CONSOLE_LINES][MAX_LINE_LENGTH];
static int line_count = 0;

static char input_buffer[MAX_LINE_LENGTH] = { 0 };
static int input_length = 0;
static int scroll_offset = 0;

static float timer = 0.0f;
static float flash_time = 0.5f;

/*
console_init()
Called once at the game start to
initialize the console
*/
void console_init() {
    line_count = 0;
    input_length = 0;
    memset(console_lines, 0, sizeof(console_lines));
    memset(input_buffer, 0, sizeof(input_buffer));

    char skyesrc_version_str[64];
    snprintf(skyesrc_version_str, sizeof(skyesrc_version_str),
         "### skyesrc v%d.%d.%d ###", 
         SKYESRC_VERSION_MAJOR, 
         SKYESRC_VERSION_MINOR, 
         SKYESRC_VERSION_PATCH
        );

    console_log(skyesrc_version_str);
    console_line();
}


/*
console_log()
Call this to print a string to the console
*/
void console_log(const char *text) {
    if (line_count >= MAX_CONSOLE_LINES) {
        for (int i = 1; i < MAX_CONSOLE_LINES; i++) {
            strcpy(console_lines[i - 1], console_lines[i]);
        }
        line_count = MAX_CONSOLE_LINES - 1;
    }
    strncpy(console_lines[line_count++], text, MAX_LINE_LENGTH);
}


/*
console_execute()
Called to execute a command by a string name
Define the commands in here
*/
static void console_execute(const char *input) {

    if (global_console_open == false) return;

    if (strncmp(input, "print ", 6) == 0) {
        console_log(input + 6);

    } else if (strcmp(input, "help") == 0) {

        console_br();
        console_log("### Help Menu ###");
        console_log("--- Commands ---");
        console_log("/help - Call to view command list and help menu.");
        console_log("\n");
        console_log("--- Cheats ---");
        console_log("/godmode - Invincibility, can't get hurt.");
        console_br();

        
    } else {
        console_log("Unknown command.");
    }
}


/*
console_update()
Called every game TICK, meant to update text, cursor,
scroll position, etc. for the console.
*/
void console_update() {

    if (IsKeyPressed(KEY_GRAVE))
    {
        global_console_open = !global_console_open;
        if (global_console_open) 
        {
            EnableCursor();
        }
        else
        {
            if (global_paused == false)
                DisableCursor();
        }

        return;
    }

    if (global_console_open == false) return;

    // Scroll with mouse wheel
    int scroll = GetMouseWheelMove(); // +1 up, -1 down
    scroll_offset -= scroll;

    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 126 && input_length < MAX_LINE_LENGTH - 1) {
            input_buffer[input_length++] = (char)key;
            input_buffer[input_length] = '\0';
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && input_length > 0) {
        input_buffer[--input_length] = '\0';
    }

       if (IsKeyPressed(KEY_ENTER)) {
        if (input_length > 0) {
            scroll_offset = 0;

            // Command or message
            if (input_buffer[0] == '/') {
                // Command
                char *command = input_buffer + 1;

                // Log as command with >
                char command_with_prefix[MAX_LINE_LENGTH + 4];
                snprintf(command_with_prefix, sizeof(command_with_prefix), "> %s", command);
                console_log(command_with_prefix);

                console_execute(command);
            } else {
                // Message
                char username[64] = "Bobler";
                char message_with_prefix[MAX_LINE_LENGTH + 10];
                snprintf(message_with_prefix, sizeof(message_with_prefix), "%s: %s", username, input_buffer);
                console_log(message_with_prefix);
            }

            input_buffer[0] = '\0';
            input_length = 0;
        }
    }

}


/*
console_draw()
Called to draw all the text and console
to the GUI
*/
void console_draw() {
    if (global_console_open == false) return;

    float delta = GetFrameTime();

    int line_height = 22;
    int max_visible_lines = (300 - 60) / line_height;
    int console_height = Clamp(line_count * line_height + 60, 100, 300);

    // Clamp scroll_offset
    if (scroll_offset < 0) scroll_offset = 0;
    if (scroll_offset > line_count - max_visible_lines)
        scroll_offset = Max(0, line_count - max_visible_lines);

    // Draw background
    DrawRectangle(0, 0, GAME_SCREEN_WIDTH, console_height, (Color){0, 0, 0, 100});

    // Draw lines from bottom up, with scroll offset
    int y = console_height - 80;
    for (int i = line_count - 1 - scroll_offset; i >= 0 && y >= 0; i--) {
        DrawText(console_lines[i], 10, y, 20, WHITE);
        y -= line_height;
        if (y < 0) break;
    }

    const char *prompt = input_length > 0 ? input_buffer : " Type /help for more options.";
    Color prompt_color = input_length > 0 ? YELLOW : GRAY;

    // Blinking cursor
    timer += delta;
    if (timer >= flash_time*2)
        timer = 0;

    char input_with_cursor[1024];
    snprintf(input_with_cursor, sizeof(input_with_cursor), "%s%s", prompt, (timer < flash_time && input_length > 0 ? "|" : " "));

    // Draw input line
    DrawText(TextFormat("> %s", input_with_cursor), 10, console_height - 30, 20, prompt_color);
    DrawText(TextFormat("  %s", input_length <= 0 && timer >= flash_time ? "|" : ""), 10, console_height - 30, 20, prompt_color);

}


/*
console_line()
Prints a line to the console for ease of access
*/
void console_line()
{
    console_log("---------------");
}


/*
console_br()
Breaks the current line on the console,
used for spacing <br/>
*/
void console_br()
{
    console_log("\n");
}