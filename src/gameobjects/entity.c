#include "entity.h" 

/*
convert_entity_origin
Converts entity origin to raylib origin / position
*/
Vector3 convert_entity_origin(Vector3 tb_origin)
{
    float scale = 0.1;
    return (Vector3){
        tb_origin.x * scale,
        tb_origin.y * scale, 
        -tb_origin.z * scale
    };
}

