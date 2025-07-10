#include "../headers/map.h"
#include "../headers/brush.h"
#include "../headers/brushface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/brushtopolygon.h"
#include "../headers/texturemanager.h"
#include "collisionbox.h"
#include "geometry.h"
#include "float.h"
#include "global.h"
#include "player.h"

Map map; // stores the currently loaded map
Geometry models[10000]; // if you have more than 10,000 map brushes, you need a second map...
CollisionBox brush_collisions[10000];
int model_count = 0;

/*
parse_map
filename[const char*] -- the filename of the map to be loaded ie. "myamazingmap.map"
-- Stores the data about the currently loaded map.
*/
int map_parse(const char* filename)
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
    int in_entity = false;
    int in_brush = false;
    int current_entity_has_brush = false;

    Brush current_brush = {0};
    int current_brushface_index = 0;

    Entity current_entity = {0};
    int current_entity_index = 0;

    // Loop through all text in the .map file
    while (fgets(line, sizeof(line), file))
    {
        char* trimmed = line;

        // trim leading whitespaces
        while(*trimmed == ' ' || *trimmed == '\t') trimmed++;

        //skip comments and empty lines
        if (trimmed[0] == '/' && trimmed[1] == '/') continue;
        if (trimmed[0] == '\n' || trimmed[0] == '\0') continue;

        /*
        Object Start {
        ----------------------------------
        */
        if (strcmp(trimmed, "{\n") == false)
        {
            if (in_entity && !in_brush)
            {
                // Brush Start
                in_brush = true;
                current_brushface_index = 0; // restart brush 
                memset(&current_brush, 0, sizeof(Brush)); //clear brush struct
            }
            else if (!in_entity)
            {
                // Entity Start
                in_entity = true;
                current_entity_index = 0; // restart entity
                memset(&current_entity, 0, sizeof(Entity));
            }
            continue;
        }
        /*
        ----------------------------------
        */


        /*
        Object End }
        ----------------------------------
        */
        if (strcmp(trimmed, "}\n") == false)
        {
            if (in_brush)
            {
                if (map.brush_count < MAX_BRUSHES) 
                    map.brushes[map.brush_count++] = current_brush;
                in_brush = false;
            }
            
            else if (in_entity)
            {
                /*
                Parse Entity Classname's
                ----------------------------------
                */
                    // info_player_start
                    if (string_equals(current_entity.classname, "info_player_start"))
                    {
                        global_player_spawn = convert_entity_origin(current_entity.origin);
                    }
                /*
                ----------------------------------
                */

                // Add Entity to array
                if (map.entity_count < MAX_ENTITIES) 
                    map.entities[map.entity_count++] = current_entity;
                in_entity = false;
            }
            continue;
        }
        /*
        ----------------------------------
        */


        /*
        Parse Entity
        ----------------------------------
        */
        if (in_entity && !in_brush)
        {
            char key[128], value[128];

            if (sscanf(trimmed, "\"%127[^\"]\" \"%127[^\"]\"", key, value) == 2) 
            {
                if (strstr(key, "classname") == 0)
                    printf("  Property: %s = %s\n", key, value);
                else
                    printf("\n  Entity: %s = %s\n", key, value);
                    

                /*
                Map Version
                ----------------------------------
                */
                if (string_equals(key, "mapversion"))
                {
                    map.mapversion = atoi(value);
                    printf("Map Version: %i \n", map.mapversion);

                    if (map.mapversion != 220){
                        printf("Only support for valve 220 .map files currently...");
                        CloseWindow();
                    }
                }
                /*
                ----------------------------------
                */


                /*
                Set Current Entity Properties
                ----------------------------------
                */
                    // classname
                    if (string_equals(key, "classname"))
                    {
                        strcpy(current_entity.classname, value);
                    }

                    // origin
                    if (string_equals(key, "origin"))
                    {
                        sscanf(value, "%f %f %f", &current_entity.origin.x, &current_entity.origin.z, &current_entity.origin.y);
                    }

                    // set optional properties
                    

                /*
                ----------------------------------
                */
            }
        }
        /*
        ----------------------------------
        */


        /*
        Parse Brush Face
        ----------------------------------
        */
        if (in_brush)
        {
            BrushFace brushface = {0};
            char texture_name[64];

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

            int SUCCESS = 21;
            if (matched == SUCCESS)
            {
                // success
                // get texture name
                strncpy(brushface.texture, texture_name, sizeof(brushface.texture));
                brushface.texture[sizeof(brushface.texture) - 1] = '\0';

                if (current_brushface_index < BRUSH_FACE_COUNT)
                {
                    current_brush.brush_faces[current_brushface_index++] = brushface;
                    current_brush.brush_face_count = current_brushface_index;
                }
            } else {
                // failed
                printf("!!! Failed to parse brush face line: %s (matched %d)\n", trimmed, matched);
            }
        }
        /*
        ----------------------------------
        */
    }

    printf("Loaded %i Geometry Brushes. \n", map.brush_count);

    //generate polygons
    printf("Generating map polygons from brushes.... \n");
    for (int i=0; i < map.brush_count; i++)
    {
        //printf("Generating Polygon: %i \n", i);
        polygon_generate_from_brush(&map.brushes[i]);
        
        // loop over all faces in brush
        for (int j= 0; j < map.brushes[i].brush_face_count; j++)
        {
            Plane plane = brushface_to_plane(map.brushes[i].brush_faces[j]);
            polygon_sort_vertices(&map.brushes[i].polys[j], (Vector3){ plane.normal.x, plane.normal.y, plane.normal.z });
        }
    }

    fclose(file);
    map_create_models();
    return true;
}


/*
map_create_models
-- creates a model from a polygonal brush
*/
void map_create_models()
{
    float scale = 0.1f;
    float rotation_degrees = 0.0f; // change to 90.0f, 180.0f, etc. if needed
    float rotation_radians = rotation_degrees * (PI / 180.0f);

    // Precompute Y-axis rotation matrix (TrenchBroom Y is Raylib Z)
    float cos_theta = cosf(rotation_radians);
    float sin_theta = sinf(rotation_radians);

    for (int i = 0; i < map.brush_count; i++)
    {
        Brush *brush = &map.brushes[i];

        for (int j = 0; j < brush->brush_face_count; j++)
        {
            BrushFace *face = &brush->brush_faces[j];
            Polygon *poly = &brush->polys[j];

            if (poly->vertex_count < 3) continue;

            Texture2D texture = texture_get_cached(face->texture);

            // Centroid calculation (raw)
            Vector3 centroid = {0};
            for (int i = 0; i < poly->vertex_count; i++) {
                centroid = Vector3Add(centroid, poly->vertices[i]);
            }
            centroid = Vector3Scale(centroid, 1.0f / poly->vertex_count);

            int triangle_count = poly->vertex_count;
            Mesh mesh = {0};
            mesh.vertexCount = triangle_count * 3;
            mesh.triangleCount = triangle_count;

            mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
            mesh.texcoords = (float *)MemAlloc(mesh.vertexCount * 2 * sizeof(float));

            int index = 0;
            for (int i = 0; i < triangle_count; i++) {
                Vector3 verts[3] = {
                    poly->vertices[(i + 1) % poly->vertex_count],
                    poly->vertices[i],
                    centroid
                };

                Vector2 uvs[3];

                switch(map.mapversion)
                {
                    default: // Standard MAP Format
                        uvs[0] = polygon_project_to_uv_standard(verts[0], face);
                        uvs[1] = polygon_project_to_uv_standard(verts[1], face);
                        uvs[2] = polygon_project_to_uv_standard(verts[2], face);
                    break;
                    
                    case 220: // Valve 220 Map Format
                        uvs[0] = polygon_project_to_uv_valve220(verts[0], face);
                        uvs[1] = polygon_project_to_uv_valve220(verts[1], face);
                        uvs[2] = polygon_project_to_uv_valve220(verts[2], face);
                    break;
                }
                

                for (int v = 0; v < 3; v++) {
                    Vector3 p = verts[v];

                    // Coordinate conversion: (x, y, z) → (x, z, -y)
                    float x = p.x;
                    float y = p.z;    // swap Y and Z
                    float z = -p.y;   // invert old Y

                    // Apply Y-axis rotation (around new Y axis, which was TrenchBroom Z)
                    float x_rot = x * cos_theta - z * sin_theta;
                    float z_rot = x * sin_theta + z * cos_theta;

                    // Apply scale
                    x_rot *= scale;
                    y *= scale;
                    z_rot *= scale;

                    // Store vertex
                    mesh.vertices[index * 3 + 0] = x_rot;
                    mesh.vertices[index * 3 + 1] = y;
                    mesh.vertices[index * 3 + 2] = z_rot;

                    mesh.texcoords[index * 2 + 0] = uvs[v].x;
                    mesh.texcoords[index * 2 + 1] = uvs[v].y;

                    index++;
                }
            }

            // AABB Bounds - Create bounding box / collision shape for simple checks
            Vector3 min = { FLT_MAX, FLT_MAX, FLT_MAX };
            Vector3 max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

            for (int v = 0; v < mesh.vertexCount; v++) {
                Vector3 vert = {
                    mesh.vertices[v * 3 + 0],
                    mesh.vertices[v * 3 + 1],
                    mesh.vertices[v * 3 + 2],
                };

                if (vert.x < min.x) min.x = vert.x;
                if (vert.y < min.y) min.y = vert.y;
                if (vert.z < min.z) min.z = vert.z;

                if (vert.x > max.x) max.x = vert.x;
                if (vert.y > max.y) max.y = vert.y;
                if (vert.z > max.z) max.z = vert.z;
            }
            BoundingBox bounds = (BoundingBox){ min, max };

            int tri_count = mesh.vertexCount / 3;

            CollisionTriangle shape = {
                .triangles = MemAlloc(sizeof(Triangle) * tri_count),
                .count = tri_count
            };

            // Triangle Collisions -- More advanced detection
            for (int i=0; i<tri_count; i++)
            {
                shape.triangles[i].a = (Vector3){
                mesh.vertices[(i * 3 + 0) * 3 + 0],
                mesh.vertices[(i * 3 + 0) * 3 + 1],
                mesh.vertices[(i * 3 + 0) * 3 + 2]
            };
            shape.triangles[i].b = (Vector3){
                mesh.vertices[(i * 3 + 1) * 3 + 0],
                mesh.vertices[(i * 3 + 1) * 3 + 1],
                mesh.vertices[(i * 3 + 1) * 3 + 2]
            };
            shape.triangles[i].c = (Vector3){
                mesh.vertices[(i * 3 + 2) * 3 + 0],
                mesh.vertices[(i * 3 + 2) * 3 + 1],
                mesh.vertices[(i * 3 + 2) * 3 + 2]
            };
            }

            UploadMesh(&mesh, false);
            Model model = LoadModelFromMesh(mesh);
            model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

            Geometry geometry;
            geometry.model = model;
            geometry.bounds = bounds;
            geometry.collision = shape;

            models[model_count++] = geometry;
            
        }
    }
}


/*
map_clear_models
-- clears all the map models from memory
-- clear memory because raylib does not
-- call this when the game ends
*/
void map_clear_models()
{
    for (int i=0; i < model_count; i++)
    {
        UnloadModel(models[i].model);
    }
}


/*
map_draw
-- draw all polygons generated in the map
*/
void map_draw()
{
    map_draw_models();
    //map_draw_model_wireframes(); // debug

}


/*
map_draw_models
-- draws each model in the model array
*/
void map_draw_models()
{
    for (int i=0; i < model_count; i++)
    {
        DrawModel(models[i].model, (Vector3){0}, 1.0f, WHITE);
        DrawModelWires(models[i].model,(Vector3){0}, 1.0f, WHITE);
        //DrawBoundingBox(models[i].bounds,PURPLE);
    }
}


/*
map_draw_model_wireframes
-- draws a debug wireframe around the map polygonal mesh
*/
void map_draw_model_wireframes()
{
    //draw wireframe reflecting the Geometry coords
    for (int i=0; i < model_count; i++)
    {
        DrawModelWires(models[i].model, (Vector3){0}, 1.0f, RED);
    }
}