#include "include/scene_world.h"
#include "include/constants.h"
#include "include/block_type.h"
#include "include/perlin.h"
#include "include/actor_player.h"
#include <sys/param.h>
#include <coelum/utils.h>
#include <coelum/actor_light.h>
#include <coelum/constants.h>
#include <coelum/draw_utils.h>
#include <string.h>
#include <glad/glad.h>


extern sprite_T* SPRITE_CROSSHAIR;

scene_world_T* init_scene_world()
{
    scene_world_T* world = calloc(1, sizeof(struct SCENE_WORLD_STRUCT));
    scene_T* s = scene_constructor((scene_T*)world, scene_world_tick, scene_world_draw, 3);
    s->bg_r = 154;
    s->bg_g = 55;
    s->bg_g = 200;

    world->GUI_state = state_constructor(init_state(), RES_WIDTH, RES_HEIGHT, 2);

    state_T* state = (state_T*) s;

    state->lighting_enabled = 1;
    
    state->camera->y = 5;
    state->camera->z = 1;
    state->camera->x = 1;
    state->camera->ry = 0;
    state->camera->projection_view->fov = 90.0f;
    projection_view_recalculate_projection(state->camera->projection_view);

    actor_light_T* light = init_actor_light(
        0.0f, 8.0f, 2.0f,
        13.0f        
    );

    world->player = init_actor_player(25.0f, NR_CHUNKS_Y * CHUNK_SIZE, 25.0f, world);

    dynamic_list_append(((state_T*)s)->actors, light);

    double heightmap[NR_CHUNKS*CHUNK_SIZE][NR_CHUNKS*CHUNK_SIZE];
    int* decormap = calloc((NR_CHUNKS*CHUNK_SIZE) * (NR_CHUNKS_Y*CHUNK_SIZE) * (NR_CHUNKS*CHUNK_SIZE), sizeof(int));

    for (int x = 0; x < NR_CHUNKS*CHUNK_SIZE; x++)
    {
        for (int z = 0; z < NR_CHUNKS*CHUNK_SIZE; z++)
        {
            double extra = perlin_get2d(x, z, 0.01f, 10.0f) + perlin_get2d(x, z, 0.009f, 20.0f) + perlin_get2d(x, z, 0.005f, 30.0f) + perlin_get2d(x, z, 0.001f, 33.0f);
            heightmap[x][z] = 1 + perlin_get2d(x, z, 0.02, 20.5f) + extra;

            int height = MAX(0, CHUNK_SIZE * heightmap[x][z]);


            int C_W = NR_CHUNKS*CHUNK_SIZE;
            int C_Y = NR_CHUNKS_Y*CHUNK_SIZE;
            
            if (height >= CHUNK_SIZE*4)
                for (int h = height; h > height-8; h--)
                    decormap[(z * (C_W) * (C_Y)) + ((h-1) * (C_W)) + x] = BLOCK_COBBLE;

            double perlin = perlin_get2d(x, z, 0.9f, 1.0f);

            // tree
            if (perlin >= 0.98f)
            {
                int tree_height = random_int(4, 6);
                int radius = random_int(3, 5);

                for (int th = 0; th < tree_height; th++)
                {
                    decormap[(z * (C_W) * (C_Y)) + ((th + height) * (C_W)) + x] = BLOCK_LOG;
                }

                for (int ry = radius; ry > 0; ry--)
                {
                    for (int rx = -radius; rx < radius; rx++)
                    {
                        for (int rz = -radius; rz < radius; rz++)
                        {
                            if (vec2_distance(rx+x, rz+z, x, z) < radius)
                            {
                                decormap[((MAX(0, MIN(C_W-1, rz+z))) * (C_W) * (C_Y)) + ((tree_height + height-ry) * (C_W)) + (MAX(0, MIN(C_W-1,rx+x)))] = BLOCK_LEAF;
                            }
                        }
                    }
                    radius -= 1;
                }
            }
        }
    }

    for (int y = 0; y < NR_CHUNKS_Y; y++)
        for (int x = 0; x < NR_CHUNKS; x++)
            for (int z = 0; z < NR_CHUNKS; z++)
                world->chunks[x][y][z] = init_chunk(x, y, z, heightmap, decormap);
    
    dynamic_list_append(((state_T*)s)->actors, (actor_T*)world->player);

    return world;
}

void scene_world_draw(scene_T* scene)
{
    scene_world_T* world = (scene_world_T*) scene;

    state_T* state = (state_T*) scene;
    
    camera_bind(state->camera);
    
    for (int y = 0; y < NR_CHUNKS_Y; y++)
    {
        for (int x = 0; x < NR_CHUNKS; x++)
        {
            for (int z = 0; z < NR_CHUNKS; z++)
            {
                int cx = x * CHUNK_SIZE;
                int cy = y * CHUNK_SIZE;
                int cz = z * CHUNK_SIZE;

                int cxc = cx + (CHUNK_SIZE / 2);
                int cyc = cy + (CHUNK_SIZE / 2);
                int czc = cz + (CHUNK_SIZE / 2);

                if (MAX(cyc, state->camera->y) - MIN(cyc, state->camera->y) > CHUNK_SIZE*2)
                    continue;
                
                if (vec2_distance(cxc, czc, state->camera->x, state->camera->z) > (CHUNK_SIZE * RENDER_DISTANCE))
                    continue;

                if (frustum_is_box_in_view(state->camera->frustum, cx, cy, cz, cx + CHUNK_SIZE, cy + CHUNK_SIZE, cz + CHUNK_SIZE))
                    chunk_draw(world->chunks[x][y][z]); 
            }
        }
    }

    draw_3D_axis(
            state->camera->frustum->center[0],
            state->camera->frustum->center[1],
            state->camera->frustum->center[2],
            128,
            128,
            128,
            state
        );

    camera_unbind(state->camera);

    actor_T* actor_player = (actor_T*) world->player;

    const char* coords_template = "x: %1.1f y: %1.1f z: %1.1f";
    char* coords_text = calloc(strlen(coords_template) + 256, sizeof(char));
    sprintf(coords_text, coords_template, actor_player->x, actor_player->y, actor_player->z);
    draw_text(
        coords_text,
        (6 + 2),
        (6 + 2),
        0,
        0,0,0,1,
        6,
        6,
        0,
        world->GUI_state 
    );
    free(coords_text);

    // TODO invert blend with background
    draw_positioned_sprite(
        SPRITE_CROSSHAIR,
        (RES_WIDTH / 2) - (SPRITE_CROSSHAIR->width / 2),
        (RES_HEIGHT / 2) - (SPRITE_CROSSHAIR->height / 2),
        0,
        0,
        0,
        world->GUI_state
    );
}

void scene_world_tick(scene_T* scene)
{
    scene_world_T* world = (scene_world_T*) scene;
    state_T* state = (state_T*) scene;

    for (int y = 0; y < NR_CHUNKS_Y; y++)
    {
        for (int x = 0; x < NR_CHUNKS; x++)
        {
            for (int z = 0; z < NR_CHUNKS; z++)
            {
                int cx = (x * CHUNK_SIZE) + (CHUNK_SIZE / 2);
                int cz = (z * CHUNK_SIZE) + (CHUNK_SIZE / 2);
                int cy = (y * CHUNK_SIZE) + (CHUNK_SIZE / 2);

                if (MAX(cy, state->camera->y) - MIN(cy, state->camera->y) > CHUNK_SIZE*2)
                    continue;
                
                if (vec2_distance(cx, cz, state->camera->x, state->camera->z) > (CHUNK_SIZE * RENDER_DISTANCE))
                    continue;

                chunk_tick(world->chunks[x][y][z]); 
            }
        }
    }
}
