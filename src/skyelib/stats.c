#include "raylib.h"
#include "SDL.h"

char sys_ram[64];
char cpu_cores[64];
char cpu_cache[64];
char platform[128];
char power_state[128];
char battery_time[64];
char power_percent[64];
char video_driver[64];

float timer = 0.0f;
float timer_max = 1.0f; // refresh every second

static int x = 10;
static int y = 10;
static int s = 20;
static int spacing = 20;
static void new_line() { y += spacing; }

void stats_draw()
{
    float delta = GetFrameTime();
    timer += delta;

    if (timer >= timer_max)
    {
        timer = 0;

        // System RAM
        snprintf(sys_ram, sizeof(sys_ram), "System RAM: %d MB", SDL_GetSystemRAM());

        // CPU Info
        snprintf(cpu_cores, sizeof(cpu_cores), "CPU Cores: %d", SDL_GetCPUCount());
        snprintf(cpu_cache, sizeof(cpu_cache), "L1 Cache: %d KB", SDL_GetCPUCacheLineSize());

        // Platform
        snprintf(platform, sizeof(platform), "Platform: %s", SDL_GetPlatform());

        // Power Info
        SDL_PowerState state = SDL_GetPowerInfo(NULL, NULL);
        switch (state) {
            case SDL_POWERSTATE_ON_BATTERY: snprintf(power_state, sizeof(power_state), "Power: On Battery"); break;
            case SDL_POWERSTATE_NO_BATTERY: snprintf(power_state, sizeof(power_state), "Power: No Battery"); break;
            case SDL_POWERSTATE_CHARGING:   snprintf(power_state, sizeof(power_state), "Power: Charging"); break;
            case SDL_POWERSTATE_CHARGED:    snprintf(power_state, sizeof(power_state), "Power: Charged"); break;
            default:                        snprintf(power_state, sizeof(power_state), "Power: Unknown"); break;
        }

        int secs = 0, pct = -1;
        SDL_GetPowerInfo(&secs, &pct);
        snprintf(battery_time, sizeof(battery_time), "Battery Time Left: %d sec", secs);
        snprintf(power_percent, sizeof(power_percent), "Battery Percent: %d%%", pct);

        // Video Driver
        snprintf(video_driver, sizeof(video_driver), "Video Driver: %s", SDL_GetCurrentVideoDriver());
    }


    x = 10;
    y = 10;
    s = 20;
    spacing = 20;

    // --- Draw stats (Raylib) ---
    DrawText("### skyesrc ###", x, y, s, WHITE); new_line();
    DrawText("---------------", x, y, s, WHITE); new_line();
    DrawText(sys_ram, x, y, s, WHITE); new_line();
    DrawText(cpu_cores, x, y, s, WHITE); new_line();
    DrawText(cpu_cache, x, y, s, WHITE); new_line();
    DrawText(platform, x, y, s, WHITE); new_line();
    DrawText(power_state, x, y, s, WHITE); new_line();
    DrawText(battery_time, x, y, s, WHITE); new_line();
    DrawText(power_percent, x, y, s, WHITE); new_line();
    DrawText(video_driver, x, y, s, WHITE);
}

