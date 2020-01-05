#ifndef VOXEL_ACTOR_PLAYER_H
#define VOXEL_ACTOR_PLAYER_H
#include <coelum/actor.h>
#include "scene_world.h"


typedef struct ACTOR_PLAYER_STRUCT
{
    actor_T base;
    float distance;
    unsigned int collision;
    unsigned int fly_mode;
    float flying_speed;
    float walking_speed;
    scene_world_T* world;
} actor_player_T;

actor_player_T* init_actor_player(float x, float y, float z, scene_world_T* world);

void actor_player_tick(actor_T* self);

void actor_player_draw(actor_T* self);
#endif
