#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include "./brushface.h"
#include "./brush.h"
#include "geometry.h"
#include "entity.h"

#define MAX_LINE 1024
#define MAX_BRUSHES 10000
#define MAX_ENTITIES 1000

extern Geometry models[];
extern int model_count;

// struct to hold the data stored in .map file
typedef struct {
    int mapversion;
    int brush_count;
    int entity_count;
    Brush brushes[MAX_BRUSHES]; //contains all the brush faces in a map
    Entity entities[MAX_ENTITIES];
} Map; 

extern Map map;

int map_parse(const char* filename);
void map_create_models();
void map_clear_models();
void map_draw();
void map_draw_model_wireframes();
void map_draw_models();

#endif // MAP_H
