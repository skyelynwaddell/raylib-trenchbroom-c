#ifndef ANIMS_H
#define ANIMS_H

// --- SHOTGUNNER ---
#define MODEL_SHOTGUNNER "monster_shotgunner/monster_shotgunner.glb"
#define ANIM_COUNT_SHOTGUNNER 12
static Texture textures_shotgunner[3];
// ----------------------------------
#define ANIM_SHOTGUNNER_CROUCH      0
#define ANIM_SHOTGUNNER_DEATH_1     1
#define ANIM_SHOTGUNNER_DEATH_2     2
#define ANIM_SHOTGUNNER_DEATH_3     3
#define ANIM_SHOTGUNNER_DEATH_4     4
#define ANIM_SHOTGUNNER_HURT_1      5
#define ANIM_SHOTGUNNER_HURT_2      6
#define ANIM_SHOTGUNNER_IDLE        7
#define ANIM_SHOTGUNNER_RUN         8
#define ANIM_SHOTGUNNER_SHOOT       9
#define ANIM_SHOTGUNNER_TPOSE       10
#define ANIM_SHOTGUNNER_WALK        11
// ----------------------------------


// --- PISTOL ARMS ---
#define MODEL_PISTOL_ARMS "arms_newpistol/arms_newpistol.glb"
static float PISTOL_MODEL_SCALE = 1.2f;
static int ANIM_COUNT_PISTOL = 6;
// ----------------------------------
static Texture viewmodel_pistol_textures[6];
static int ANIM_PISTOL_IDLE = 0;
static int ANIM_PISTOL_RELOAD = 2;
// ----------------------------------


#endif // ANIMS_H