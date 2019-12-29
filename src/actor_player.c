#include "include/actor_player.h"
#include "include/chunk.h"
#include "include/block_type.h"
#include <coelum/physics.h>
#include <coelum/current.h>
#include <coelum/input.h>

#define NR_CHUNKS 16

extern keyboard_state_T* KEYBOARD_STATE;
extern mouse_state_T* MOUSE_STATE;
extern chunk_T* chunks[NR_CHUNKS][1][NR_CHUNKS];

actor_player_T* init_actor_player(float x, float y, float z)
{
    actor_player_T* actor_player = calloc(1, sizeof(struct ACTOR_PLAYER_STRUCT));
    actor_T* actor = actor_constructor(
        (actor_T*)actor_player,
        x, y, z,
        actor_player_tick, actor_player_draw, "player"
    );
    actor_player->distance = 0.0f;
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

    physics_to_zero(&self->dx, self->friction);
    physics_to_zero(&self->dy, 0.009f);
    physics_to_zero(&self->dz, self->friction);

    float wspeed = 0.1f;
    
    if (KEYBOARD_STATE->keys[GLFW_KEY_W])
    {
        self->dx += cos(glm_rad(state->camera->ry + 90)) * wspeed;
        self->dz -= sin(glm_rad(state->camera->ry + 90)) * wspeed;
        actor_player->distance += 0.3f;
    }

    if (KEYBOARD_STATE->keys[GLFW_KEY_S])
    {
        self->dx -= cos(glm_rad(state->camera->ry + 90)) * wspeed;
        self->dz += sin(glm_rad(state->camera->ry + 90)) * wspeed;
    }

    if (KEYBOARD_STATE->keys[GLFW_KEY_SPACE])
    {
        physics_vec2_push(&self->dx, &self->dy, -90.0f, 0.1f);
    }

    state->camera->x = self->x + 0.5f;
    state->camera->z = self->z + 0.5f;
    state->camera->y = (self->y - 0.5f) - (cos(actor_player->distance) * 0.1f);

    state->camera->offset_x = state->camera->x;
    state->camera->offset_y = state->camera->y;
    state->camera->offset_z = state->camera->z;
    state->camera->rx += MOUSE_STATE->dy * 0.25f;
    state->camera->ry += MOUSE_STATE->dx * 0.25f;

    actor_player_move(self, self->dx, self->dy, self->dz);
}

void actor_player_draw(actor_T* actor)
{
}

void actor_player_move(actor_T* actor, float xa, float ya, float za)
{
    if (xa != 0 && ya != 0 && za != 0)
    {
        actor_player_move(actor, xa, 0, 0);
        actor_player_move(actor, 0, ya, 0);
        actor_player_move(actor, 0, 0, za);

        return;
    }

    float px = actor->x + xa;
    float py = actor->y + ya;
    float pz = actor->z + za;

    chunk_T* chunk = chunks[(int)px / (16)][/*(int)py / (16 * 1)*/0][(int)pz / (16)];

    unsigned int is_colliding = 0;

    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                float bx = (chunk->x * CHUNK_SIZE) + x;
                float by = (chunk->y * CHUNK_SIZE) + y;
                float bz = (chunk->z * CHUNK_SIZE) + z;
                float w = 1;

                if (chunk->blocks[x][y][z] == BLOCK_AIR)
                    continue;

                if (px + actor->width >= bx && px <= bx + w)
                {
                    if (pz + actor->width >= bz && pz <= bz + w)
                    {
                        if ((py - actor->height >= by && py - actor->height <= by + w) || (py-0.3f >= by && py-1 <= by+w))
                        {
                            is_colliding = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    unsigned int is_ground_below = 0;
    px = actor->x;
    py = (actor->y + ya) - actor->height;
    pz = actor->z;
    
    chunk = chunks[(int)px / (16)][/*(int)py / (16 * 1)*/0][(int)pz / (16)];

    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                float bx = (chunk->x * CHUNK_SIZE) + x;
                float by = (chunk->y * CHUNK_SIZE) + y;
                float bz = (chunk->z * CHUNK_SIZE) + z;
                float w = 1;

                if (chunk->blocks[x][y][z] == BLOCK_AIR)
                    continue;

                if (px + actor->width >= bx && px <= bx + w)
                {
                    if (pz + actor->width >= bz && pz <= bz + w)
                    {
                        if (py >= by && py <= by + w)
                        {
                            is_ground_below = 1;
                            break;
                        }
                    }
                }
            }
        }
    } 

    if (!is_colliding)
    {
        actor->y += ya;
        actor->x += xa;
        actor->z += za; 
    }
    else
    {
        actor->dx = 0;
        //actor->dy = 0;
        actor->dz = 0;
    }

    if (!is_ground_below)
    {
        // gravity
        physics_vec2_push(&actor->dx, &actor->dy, 90.0f, 0.01f);
    }
    else
    {
        actor->dy = 0;
    }
}
