#ifndef MAP_H
#define MAP_H

#include "skyelib.h"
#include "lights.h"

#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

// struct to hold the data stored in .map file
typedef struct Map {
    char filename[128];
    int mapversion;

    Brush brushes[MAX_BRUSHES]; //contains all the brush faces in a map
    int brush_count;

    Geometry models[MAX_BRUSHES]; // contains all brushes converted to raylib models
    CollisionBox brush_collisions[MAX_BRUSHES]; // contains all model collision shapes
    int model_count;

    Entity entities[MAX_ENTITIES]; // contains all the map objects/entities
    int entity_count;

    LightObject lights[MAX_LIGHTS]; // contains all the map light objects
    int light_count;

} Map; 

extern Map map;

int map_parse(const char* filename);
void map_create_models();
void map_clear_models();
void map_draw();
void map_draw_models();
void map_draw_model(Model model);

void map_hotreload();
time_t get_file_mod_time(const char *filename);

#endif // MAP_H
