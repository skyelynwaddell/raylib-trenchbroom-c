#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include "./brushface.h"
#include "./brush.h"

#define MAX_LINE 1024
#define MAX_BRUSHES 1024

// struct to hold the data stored in .map file
typedef struct {
    int mapversion;

    int brush_count;
    Brush brushes[MAX_BRUSHES]; //contains all the brush faces in a map
} Map; 

extern Map map;

int parse_map(const char* filename);
int string_equals(char* string, char* string_to_compare_to);
Vector3 convert_trenchbroom_to_raylib_axis(Vector3 v);

#endif // MAP_H
