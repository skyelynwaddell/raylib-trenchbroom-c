#include "stdio.h"
#include "../headers/brushtopolygon.h"

#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

#include "../headers/defs.h"
#include "../headers/global.h"

#include "../headers/brush.h"
#include "../headers/polygon.h"
#include "../headers/map.h"

/*
polygon_get_intersection
- gets the intersecting points from a polygon to clip them
*/
bool polygon_get_intersection(
    Vector3 n1,
    Vector3 n2,
    Vector3 n3,
    float d1,
    float d2,
    float d3,
    Vector3* out
)
{
    Vector3 c1 = Vector3CrossProduct(n2, n3);
    Vector3 c2 = Vector3CrossProduct(n3, n1);
    Vector3 c3 = Vector3CrossProduct(n1, n2);

    float denom = Vector3DotProduct(n1, c1);
    if (denom == 0.0f) return false;

    Vector3 term1 = Vector3Scale(c1, -d1);
    Vector3 term2 = Vector3Scale(c2, -d2);
    Vector3 term3 = Vector3Scale(c3, -d3);

    Vector3 sum = Vector3Add(Vector3Add(term1, term2), term3);
    *out = Vector3Scale(sum, 1.0f / denom);
    return true;
}


/*
polygon_generate_from_brush
- generates a polygon from a brush type
*/
void polygon_generate_from_brush(Brush *brush)
{
    printf("Setting vertex counts to 0...\n");
    for (int i=0; i < brush->brush_face_count; i++)
    {
        brush->polys[i].vertex_count = 0;
    }

    for (int i=0; i < brush->brush_face_count - 2; i++){
        for (int j = i+1; j < brush->brush_face_count -1; j++){
            for (int k = j + 1; k < brush->brush_face_count; k++){
                printf("Generating plane: ");

                //create planes from brush faces
                Plane plane_i = brushface_to_plane(brush->brush_faces[i]);
                Plane plane_j = brushface_to_plane(brush->brush_faces[j]);
                Plane plane_k = brushface_to_plane(brush->brush_faces[k]);


                //check for intersecting planes in the polygon
                Vector3 p;
                if (!polygon_get_intersection(
                    plane_i.normal, plane_j.normal, plane_k.normal,
                    plane_i.distance, plane_j.distance, plane_k.distance, &p
                )) 
                {
                    printf("FAILED.\n");
                    continue;
                }

                //check inside all brush planes
                bool legal = true;
                for (int m=0; m < brush->brush_face_count; m++)
                {
                    Plane test_plane = brushface_to_plane(brush->brush_faces[m]);
                    if (Vector3DotProduct(test_plane.normal, p) + test_plane.distance < -0.001f)
                    {
                        //point is outside reject it
                        printf("FAILED.\n");
                        legal = false;
                        break;
                    }
                }

                // this generation of polygon is valid
                if (legal)
                {
                    int success = 0; // checks if the plane creating was success

                    ///checks for i
                    if (!polygon_has_vertex(&brush->polys[i],p) && (brush->polys[i].vertex_count < MAX_VERTICES_PER_FACE))
                    {
                        success = 1;
                        brush->polys[i].vertices[brush->polys[i].vertex_count++] = p;
                    }


                    ///checks for j
                    if (!polygon_has_vertex(&brush->polys[j],p) && brush->polys[j].vertex_count < MAX_VERTICES_PER_FACE)
                    {
                        success = 1;
                        brush->polys[j].vertices[brush->polys[j].vertex_count++] = p;
                    }


                    ///checks for k
                    if (!polygon_has_vertex(&brush->polys[k],p) && brush->polys[k].vertex_count < MAX_VERTICES_PER_FACE)
                    {
                        success = 1;
                        brush->polys[k].vertices[brush->polys[k].vertex_count++] = p;
                    }

                    /// print if the creation was successful or not
                    if (success == 1) printf("SUCCESS.\n");
                    else printf("FAILED.\n");
                }
            }
        }
    }
}


/*
polygon_draw_all
-- draw all polygons generated in the map
*/
void polygon_draw_all()
{
    polygon_draw_wireframe(); // debug
    polygon_draw_uvs();
}


/*
polygon_draw_uvs
-- draws the uvs onto each polygon plane
*/
void polygon_draw_uvs()
{
    //loop through all brushes in the map
    for (int i=0; i<map.brush_count; i++)
    {
        Brush *brush = &map.brushes[i];

        // loop through each brush face per brush
        for (int j=0; j<brush->brush_face_count; j++)
        {
            BrushFace *face = &brush->brush_faces[j];
            Polygon *poly = &brush->polys[j];

            // validate if this poly has atleast 3 vertices.
            if (poly->vertex_count < 3) return;

            Texture2D texture = texture_get_cached(face->texture);

            // Calculate centroid
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
                Vector3 v0 = centroid;
                Vector3 v1 = poly->vertices[i];
                Vector3 v2 = poly->vertices[(i + 1) % poly->vertex_count];

                Vector2 uv0 = polygon_project_to_uv(v0, face);
                Vector2 uv1 = polygon_project_to_uv(v1, face);
                Vector2 uv2 = polygon_project_to_uv(v2, face);

                Vector2 uvs[3] = { uv0, uv1, uv2 };
                polygon_normalize_uv(uvs, 3);

                // v0
                mesh.vertices[index*3 + 0] = v0.x;
                mesh.vertices[index*3 + 1] = v0.y;
                mesh.vertices[index*3 + 2] = v0.z;
                mesh.texcoords[index*2 + 0] = uvs[0].x;
                mesh.texcoords[index*2 + 1] = uvs[0].y;
                index++;

                // v1
                mesh.vertices[index*3 + 0] = v1.x;
                mesh.vertices[index*3 + 1] = v1.y;
                mesh.vertices[index*3 + 2] = v1.z;
                mesh.texcoords[index*2 + 0] = uvs[1].x;
                mesh.texcoords[index*2 + 1] = uvs[1].y;
                index++;

                // v2
                mesh.vertices[index*3 + 0] = v2.x;
                mesh.vertices[index*3 + 1] = v2.y;
                mesh.vertices[index*3 + 2] = v2.z;
                mesh.texcoords[index*2 + 0] = uvs[2].x;
                mesh.texcoords[index*2 + 1] = uvs[2].y;
                index++;
            }

            UploadMesh(&mesh, false);
            Model model = LoadModelFromMesh(mesh);
            model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

            DrawModel(model, (Vector3){0}, 1.0f, WHITE);

            // free raylib model mem
            UnloadModel(model);
        }
    }
}


/*
polygon_draw_wireframe
-- draws a debug wireframe around the polygonal mesh
*/
void polygon_draw_wireframe()
{
    /// DRAWS DEBUG WIREFRAME OF THE OBJECT
        for (int i = 0; i < map.brush_count; i++)
        {
            Brush *brush = &map.brushes[i];

            //printf("Attempting to draw brush...");
            for (int j = 0; j < brush->brush_face_count; j++)
            {
                //printf("Brushface data:\n");
                //brushface_print(brush->brush_faces[j], j);
                Polygon *poly = &brush->polys[j];

                for (int k = 0; k < poly->vertex_count; k++)
                {
                    Vector3 a = poly->vertices[k];
                    Vector3 b = poly->vertices[(k + 1) % poly->vertex_count];
                    DrawLine3D(a, b, RED);
                    DrawPoint3D(a, BLUE);
                }
            }
        }
}


/*
polygon_sort_vertices
-- sorts all the maps vertices in polygons, so shapes arent all messed up and actually 
represent what they need to look like
*/
void polygon_sort_vertices(Polygon* poly, Vector3 normal)
{
    if (poly->vertex_count < 3) return; // nothing to sort

    //calculate centroid
    Vector3 centroid = { 0, 0, 0 };
    for (int i=0; i<poly->vertex_count; i++)
    {
        centroid = Vector3Add(centroid, poly->vertices[i]);
    }

    centroid = Vector3Scale(centroid, 1.0f / poly->vertex_count);

    //compute polygon normal
    Vector3 edge1 = Vector3Subtract(poly->vertices[1], poly->vertices[0]);
    Vector3 edge2 = Vector3Subtract(poly->vertices[2], poly->vertices[0]);
    //Vector3 normal = Vector3Normalize(Vector3CrossProduct(edge1,edge2));
    Vector3 ref_vec = Vector3Normalize(edge1);


    //temporarily store angles of each vertex relative to centroid and ref_vec
    typedef struct {
        float angle;
        Vector3 vertex;
    } AngleVertex;

    AngleVertex arr[MAX_VERTICES_PER_FACE];

    for (int i=0; i< poly->vertex_count; i++)
    {
        Vector3 dir = Vector3Subtract(poly->vertices[i], centroid);

        // project dir to plane perpendicular to normal:
        // dir_proj = dir - (dir ⋅ normal)*normal

        float dist_to_normal = Vector3DotProduct(dir, normal);
        Vector3 proj = Vector3Subtract(dir, Vector3Scale(normal, dist_to_normal));
        proj = Vector3Normalize(proj);

        // compute angle between refVec and proj using atan2
        // atan2 returns angle from refVec to proj in range [-pi, pi]
        Vector3 cross = Vector3CrossProduct(ref_vec, proj);
        float dot = Vector3DotProduct(ref_vec, proj);
        float angle = atan2f(Vector3DotProduct(cross, normal), dot);

        arr[i].angle = angle;
        arr[i].vertex = poly->vertices[i];
    }

    // sort vertices by angle ascending
    // simple insertion sort
    for (int i = 1; i < poly->vertex_count; i++)
    {
        AngleVertex key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].angle > key.angle)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }

    // copy sorted vertices back to polygon
    for (int i = 0; i < poly->vertex_count; i++)
    {
        poly->vertices[i] = arr[i].vertex;
    }

}


/*
polygon_project_to_uv
-- get the UV data from a brushface
*/
Vector2 polygon_project_to_uv(Vector3 point, BrushFace *face)
{
    
    Vector3 s = Vector3Normalize((Vector3){ face->uv_s.x, face->uv_s.y, face->uv_s.z });
    Vector3 t = Vector3Normalize((Vector3){ face->uv_t.x, face->uv_t.y, face->uv_t.z });

    float s_offset = face->uv_s.w;
    float t_offset = face->uv_t.w;

    float u = Vector3DotProduct(point, s) + s_offset;
    float v = Vector3DotProduct(point, t) + t_offset;

    u /= face->u_scale;
    v /= face->v_scale;

    return (Vector2){u,v};
}


/*
polygon_normalize_uv
-- normalizes the UV
*/
void polygon_normalize_uv(Vector2 *uvs, int count)
{
    //normalize u
    float nearest_u = uvs[0].x;
    for (int i=0; i<count; i++)
    {
        if (fabsf(uvs[i].x) > 1 && fabsf(uvs[i].x) < fabsf(nearest_u))
        {
            nearest_u = uvs[i].x;
        }
    }

    if (fabsf(nearest_u) > 1)
    {
        for (int i=0; i<count; i++)
        {
            uvs[i].x -= nearest_u;
        }
    }

    //normalize v
    float nearest_v = uvs[0].y;
    for (int i=0; i<count; i++)
    {
        if (fabsf(uvs[i].y) > 1 && fabsf(uvs[i].y) < fabsf(nearest_v))
        {
            nearest_v = uvs[i].y;
        }
    }

    if (fabsf(nearest_v) > 1)
    {
        for (int i=0; i<count; i++)
        {
            uvs[i].y -= nearest_v;
        }
    }
}


/*
polygon_has_vertex
-- checks if the polygon already has vertex so we can check which polygons need texturing.
*/
bool polygon_has_vertex(Polygon *poly, Vector3 v)
{
    for (int i = 0; i < poly->vertex_count; i++)
    {
        if (Vector3Length(Vector3Subtract(poly->vertices[i], v)) < 0.001f)
            return true;
    }
    return false;
}
