#include "include/actor_player.h"
#include "include/chunk.h"
#include "include/block_type.h"
#include "include/constants.h"
#include <coelum/physics.h>
#include <coelum/current.h>
#include <coelum/input.h>
#include <sys/param.h>


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
    actor_player->fly_mode = 0;
    actor_player->flying_speed = 0.2f;
    actor_player->walking_speed = 0.1f;
    actor_player->world = world;
    actor_player->collision = 0;
    actor->width = 1;
    actor->height = 2;
    actor->friction = 0.20f;

    return actor_player;
}

static unsigned int collides(actor_T* self, float dx, float dy, float dz, float ox, float oy, float oz)
{
    if (dx != 0 && dy != 0 && dz != 0)
    {
        if (collides(self, dx, 0, 0, ox, oy, oz))
            return 1;
        if (collides(self, 0, dy, 0, ox, oy, oz))
            return 1;
        if (collides(self, 0, 0, dz, ox, oy, oz))
            return 1;
        
        return 0;
    }

    actor_player_T* player = (actor_player_T*) self;
    player->collision = 0;

    float px = (self->x) + dx;
    float py = ((self->y) + oy) + dy;
    float pz = (self->z) + dz;

    int chunk_x = px / CHUNK_SIZE;
    int chunk_y = py / CHUNK_SIZE;
    int chunk_z = pz / CHUNK_SIZE;

    int ipx = (int)px;
    int ipy = (int)py;
    int ipz = (int)pz;

    int w = 1;

    if (player->world->chunks[chunk_x][chunk_y][chunk_z]->blocks[ipx % 16][ipy % 16][ipz % 16] != BLOCK_AIR)
        return 1;
    else
        return 0;

    /*if (chunk->blocks[MAX(0, MIN(CHUNK_SIZE-1, x))][MAX(0, MIN(CHUNK_SIZE-1,y))][MAX(0, MIN(CHUNK_SIZE-1,z))] == BLOCK_AIR)
        return 1;
    else
        return 0;*/

    /*if (px + self->width >= bx && px <= bx + w)
    {
        if (pz + self->width >= bz && pz <= bz + w)
        {
            if (py + self->height >= by && py <= by + w)
            {
                player->collision = 1;
                break;
            }
        }
    }

    return player->collision;*/ 
}

void actor_player_tick(actor_T* self)
{
    actor_player_T* actor_player = (actor_player_T*) self;
    scene_T* scene = get_current_scene();
    state_T* state = (state_T*) scene; 

    state->camera->x = self->x;/* + 0.5f;*/
    state->camera->z = self->z;/* + 0.5f;*/

    state->camera->offset_x = state->camera->x;
    state->camera->offset_y = state->camera->y;
    state->camera->offset_z = state->camera->z;
    state->camera->rx += MOUSE_STATE->dy * 0.25f;
    state->camera->ry += MOUSE_STATE->dx * 0.25f; 

    if (actor_player->fly_mode)
    {
        physics_tick_actor(self);

        state->camera->y = self->y;

        if (KEYBOARD_STATE->keys[GLFW_KEY_W])
        {
            self->dx += cos(glm_rad(state->camera->ry + 90)) * actor_player->flying_speed;
            self->dz -= sin(glm_rad(state->camera->ry + 90)) * actor_player->flying_speed;
            self->dy -= cos(glm_rad(state->camera->rx + 90)) * actor_player->flying_speed;
            actor_player->distance += 0.3f;
        }
    }
    else
    {
        physics_to_zero(&self->dx, self->friction);
        //physics_to_zero(&self->dy, 0.2f);
        physics_to_zero(&self->dz, self->friction);

        // head bobbing 
        state->camera->y = (self->y - 0.5f) - (cos(actor_player->distance) * 0.1f);

        if (KEYBOARD_STATE->keys[GLFW_KEY_W])
        {
            self->dx += cos(glm_rad(state->camera->ry + 90)) * actor_player->walking_speed;
            self->dz -= sin(glm_rad(state->camera->ry + 90)) * actor_player->walking_speed;
            actor_player->distance += 0.3f;
        }
        
        if (!collides(self, 0, self->dy, 0, 0, -self->height, 0))
        {
            self->dy -= 0.02f;
        }
        else
        {
            if (KEYBOARD_STATE->keys[GLFW_KEY_SPACE])
            {
                self->dy += 0.2f;
            }
            else
            {
                self->dy = 0;
            }
        }
        
        if (!collides(self, self->dx, 0, 0, 0, 0, 0) && !collides(self, self->dx, 0, 0, 0, -1, 0))
        {
            self->x += self->dx;
        }
        else
        {
            self->dx = 0;
        }

        if (!collides(self, 0, self->dy, 0, 0, 0, 0))
        {
            self->y += self->dy;
        }
        else
        {
            self->dy = 0;
        }

        if (!collides(self, 0, 0, self->dz, 0, 0, 0) && !collides(self, 0, 0, self->dz, 0, -1, 0))
        {
            self->z += self->dz;
        }
        else
        {
            self->dz = 0;
        }
    }

    int chunk_x = (self->x + 0.5f) / CHUNK_SIZE;
    int chunk_y = self->y / CHUNK_SIZE;
    int chunk_z = (self->z + 0.5f) / CHUNK_SIZE;

    chunk_T* chunk = actor_player->world->chunks[chunk_x][chunk_y][chunk_z];

    chunk->selected = 1;
}

void actor_player_draw(actor_T* actor)
{
}
