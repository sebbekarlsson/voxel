#include "include/actor_player.h"
#include "include/chunk.h"
#include "include/block_type.h"
#include "include/constants.h"
#include <coelum/physics.h>
#include <coelum/current.h>
#include <coelum/input.h>


extern keyboard_state_T* KEYBOARD_STATE;
extern mouse_state_T* MOUSE_STATE;
extern chunk_T* chunks[NR_CHUNKS][NR_CHUNKS_Y][NR_CHUNKS];

actor_player_T* init_actor_player(float x, float y, float z, scene_world_T* world)
{
    actor_player_T* actor_player = calloc(1, sizeof(struct ACTOR_PLAYER_STRUCT));
    actor_T* actor = actor_constructor(
            (actor_T*)actor_player,
            x, y, z,
            actor_player_tick, actor_player_draw, "player"
            );
    actor_player->distance = 0.0f;
    actor_player->fly_mode = 1;
    actor_player->walking_speed = 0.1f;
    actor_player->world = world;
    actor->width = 1;
    actor->height = 2;
    actor->friction = 0.1f;

    return actor_player;
}

void actor_player_tick(actor_T* self)
{
    actor_player_T* actor_player = (actor_player_T*) self;
    scene_T* scene = get_current_scene();
    state_T* state = (state_T*) scene; 

    state->camera->x = self->x + 0.5f;
    state->camera->z = self->z + 0.5f;

    state->camera->offset_x = state->camera->x;
    state->camera->offset_y = state->camera->y;
    state->camera->offset_z = state->camera->z;
    state->camera->rx += MOUSE_STATE->dy * 0.25f;
    state->camera->ry += MOUSE_STATE->dx * 0.25f;

    // head bobbing 
    state->camera->y = (self->y - 0.5f) - (cos(actor_player->distance) * 0.1f);

    if (KEYBOARD_STATE->keys[GLFW_KEY_W])
    {
        self->dx += cos(glm_rad(state->camera->ry + 90)) * actor_player->walking_speed;
        self->dz -= sin(glm_rad(state->camera->ry + 90)) * actor_player->walking_speed;
        self->dy -= cos(glm_rad(state->camera->rx + 90)) * actor_player->walking_speed;
        actor_player->distance += 0.3f;
    }

    physics_tick_actor(self);
    
    int chunk_x = (self->x + 0.5f) / CHUNK_SIZE;
    int chunk_y = self->y / CHUNK_SIZE;
    int chunk_z = (self->z + 0.5f) / CHUNK_SIZE;

    chunk_T* chunk = actor_player->world->chunks[chunk_x][chunk_y][chunk_z];

    chunk->selected = 1;
}

void actor_player_draw(actor_T* actor)
{
}
