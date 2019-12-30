#ifndef VOXEL_ACTOR_PLAYER_H
#define VOXEL_ACTOR_PLAYER_H
#include <coelum/actor.h>


typedef struct ACTOR_PLAYER_STRUCT
{
    actor_T base;
    float distance;
    unsigned int fly_mode;
} actor_player_T;

actor_player_T* init_actor_player(float x, float y, float z);

void actor_player_tick(actor_T* self);

void actor_player_draw(actor_T* self);

void actor_player_move(actor_T* actor, float xa, float ya, float za);
#endif
