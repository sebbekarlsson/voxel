#ifndef VOXEL_SCENE_WORLD_H
#define VOXEL_SCENE_WORLD_H
#include <coelum/scene.h>
#include "chunk.h"


typedef struct SCENE_WORLD_STRUCT
{
    scene_T base;
    chunk_T* chunks[NR_CHUNKS][NR_CHUNKS_Y][NR_CHUNKS];
    state_T* GUI_state;
    struct ACTOR_PLAYER_STRUCT* player;
} scene_world_T;

scene_world_T* init_scene_world();

void scene_world_tick(scene_T* scene);

void scene_world_draw(scene_T* scene);
#endif
