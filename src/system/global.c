#include "../headers/global.h"
#include "string.h"

int texture_cache_count = 0;
Texture2D default_texture;
TextureCacheEntry texture_cache[MAX_TEXTURES];


/*
texture_get_default
-- gets the default texture for the game, usually returned when no texture is found to prevent a game crash
*/
Texture2D texture_get_default()
{
    return default_texture;
}


/*
texture_get_cached
*texture_name [char] - the texture filename to be loaded ie. "myamazingimage"
!!!!! expects the image to be in /textures and of filetype .png !!!!!!
-- this function is called to search the cache for a texture, or adds it if not found 
-- and then returns the texture2d of the texture name provided
*/
Texture2D texture_get_cached(char *texture_name)
{
    for (int i=0; i<texture_cache_count; i++)
    {
        if (strcmp(texture_cache[i].name, texture_name) == 0)
        {
            return texture_cache[i].texture;
        }
    }

    if (texture_cache_count >= MAX_TEXTURES)
    {
        
        printf("Texture Cache full");
        return texture_get_default();
    }

    char path[128];
    snprintf(path, sizeof(path), "textures/%s.png", texture_name);

    Texture2D tex = LoadTexture(path);
    if (tex.id == 0)
    {
        printf("Failed to load texture: %s", path);
        return texture_get_default();
    }

    strncpy(texture_cache[texture_cache_count].name, texture_name, 63);
    texture_cache[texture_cache_count].texture = tex;
    return tex;
}