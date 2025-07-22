#ifndef WEAPONS_H
#define WEAPONS_H

#include "skyelib.h"

typedef enum WEAPON_TYPE {
    WEAPON_TYPE_NONE,
    WEAPON_TYPE_PISTOL
} WEAPON_TYPE;

typedef enum AMMO_POOL_TYPE {
    AMMO_POOL_TYPE_NONE,
    AMMO_POOL_TYPE_9MM
} AMMO_POOL_TYPE;

typedef struct Weapon {
    AMMO_POOL_TYPE ammo_pool_type;
    int clip_size;
    int mag_size;
    float anim_duration;
    float shoot_cooldown;

    const char *model_filepath;
    float model_scale;
    int anim_count;
    Texture model_textures[24];
    int anim_idle;
    int anim_reload;
    int anim_shoot;

    Vector3 position;
    float rotation;

    int automatic;

} Weapon;

extern WEAPON_TYPE current_weapon;
extern WEAPON_TYPE players_weapons[255]; // holds the weapons the player has picked up
extern int player_weapon_count;

extern Weapon weapons[64]; // holds all the weapons in the game
extern int weapon_count;

// ammo pools
extern int ammo_pool_9mm[255];


#endif // WEAPONS_H