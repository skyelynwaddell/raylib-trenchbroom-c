#include "../headers/map.h"
#include "../headers/brush.h"
#include "../headers/brushface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/brushtopolygon.h"

Map map;

/*
parse_map
filename[const char*] -- the filename of the map to be loaded ie. "myamazingmap.map"
-- Stores the data about the currently loaded map.
*/
int parse_map(const char* filename)
{
    printf("\n \n");
    printf("Loading map data...\n");
    char fullpath[256];

    // add maps/ filepath to the filename
    snprintf(fullpath, sizeof(fullpath), "maps/%s", filename);

    FILE* file = fopen(fullpath, "r");
    if (!file)
    {
        perror("Failed to open .map file!");
        printf("Tried searching in: %s", fullpath);
        return false;
    }

    printf("Successfully opened map: %s\n", fullpath);

    char line[MAX_LINE];
    int in_entity = 0;
    int in_brush = 0;
    Brush current_brush = {0};
    int current_brushface_index = 0;


    while (fgets(line, sizeof(line), file))
    {
        char* trimmed = line;

        // trim leading whitespaces
        while(*trimmed == ' ' || *trimmed == '\t') trimmed++;

        //skip comments and empty lines
        //trimmed[strcspn(trimmed, "\r\n")] = 0;
        if (trimmed[0] == '/' && trimmed[1] == '/') continue;
        if (trimmed[0] == '\n' || trimmed[0] == '\0') continue;

        if (strcmp(trimmed, "{\n") == 0)
        {
            if (in_entity && !in_brush)
            {
                in_brush = 1;
                current_brushface_index = 0; // restart brush 
                memset(&current_brush, 0, sizeof(Brush)); //clear brush struct
                //printf(" Brush start\n");
            }
            else if (!in_entity)
            {
                in_entity = 1;
                //printf("Entity start\n");
            }
            continue;
        }

        if (strcmp(trimmed, "}\n") == 0)
        {
            if (in_brush)
            {
                if (map.brush_count < MAX_BRUSHES)
                {
                    map.brushes[map.brush_count++] = current_brush;
                    printf("Created BRUSH %i \n \n", map.brush_count-1);
                }
                in_brush = 0;
                //printf(" Brush end\n");
            }
            else if (in_entity)
            {
                in_entity = 0;
                //printf("Entity end\n");
            }
            continue;
        }

        //get key value pairs
        if (in_entity && !in_brush)
        {
            char key[128], value[128];

            if (sscanf(trimmed, "\"%127[^\"]\" \"%127[^\"]\"", key, value) == 2) 
            {
                //printf("  Property: %s = %s\n", key, value);

                // Get the Map Version of the Map file
                if (string_equals(key, "mapversion"))
                {
                    map.mapversion = atoi(value);
                    printf("Map Version: %i \n", map.mapversion);
                }
            }
        }


        //parse brush face
        if (in_brush)
        {
            BrushFace brushface = {0};
            char texture_name[64];
            //printf("   Brush Face: %s", trimmed);

            int matched = sscanf(trimmed,
                "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %63s [ %f %f %f %f ] [ %f %f %f %f ] %i %i %i",
                &brushface.pos_1.x, &brushface.pos_1.y, &brushface.pos_1.z,
                &brushface.pos_2.x, &brushface.pos_2.y, &brushface.pos_2.z,
                &brushface.pos_3.x, &brushface.pos_3.y, &brushface.pos_3.z,
                texture_name,
                &brushface.uv_s.x, &brushface.uv_s.y, &brushface.uv_s.z, &brushface.uv_s.w,
                &brushface.uv_t.x, &brushface.uv_t.y, &brushface.uv_t.z, &brushface.uv_t.w,
                &brushface.uv_rotation, &brushface.u_scale, &brushface.v_scale
            );

            // success
            if (matched == 21)
            {
                // get texture name
                strncpy(brushface.texture, texture_name, sizeof(brushface.texture));
                brushface.texture[sizeof(brushface.texture) - 1] = '\0';

                if (current_brushface_index < BRUSH_FACE_COUNT)
                {
                    current_brush.brush_faces[current_brushface_index++] = brushface;
                    current_brush.brush_face_count = current_brushface_index;
                    brushface_print(brushface, current_brushface_index);
                }
            } else {
                //failed
                printf("!!! Failed to parse brush face line: %s (matched %d)\n", trimmed, matched);
            }
        }
    }

    printf("Loaded %i brushes. \n", map.brush_count);

    //generate polygons
    printf("Generating map polygons from brushes.... \n");
    for (int i=0; i < map.brush_count; i++)
    {
        printf("Generating Polygon: %i \n", i);
        polygon_generate_from_brush(&map.brushes[i]);
        
        // loop over all faces in brush
        for (int j= 0; j < map.brushes[i].brush_face_count; j++)
        {
            Plane plane = brushface_to_plane(map.brushes[i].brush_faces[j]);
            polygon_sort_vertices(&map.brushes[i].polys[j], plane.normal);
        }
    }

    fclose(file);
    return true;
}

/*
string_equals
string[char* string] - The first string to be compared
string[char* string_to_compare_to] - the second string that is compared to the first string
-- compares two strings to see if they match
*/
int string_equals(char* string, char* string_to_compare_to)
{
    if (strcmp(string, string_to_compare_to) == 0) return 1;
    return 0;
}