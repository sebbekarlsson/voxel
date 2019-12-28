#include "include/chunk.h"
#include "include/block_type.h"
#include "include/draw.h"
#include <coelum/current.h>
#include <coelum/textures.h>
#include <coelum/draw_utils.h>
#include <coelum/utils.h>
#include <math.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <sys/param.h>


#define CHUNK_SIZE 16

extern texture_T* TEXTURE_COBBLE;


static void generate_vertices(chunk_T* chunk)
{
    chunk->vertices_size = 0;
    chunk->indices_size = 0;
    chunk->vertices = (void*)0;
    chunk->indices = (void*)0;
    chunk->block_count = 0;

    int icount = 0;

    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                /*if (x != 0 && y != 0 && z != 0 && x != CHUNK_SIZE - 1 && y != CHUNK_SIZE - 1 && z != CHUNK_SIZE - 1)
                if (
                   chunk->blocks[MAX(0, x-1)][y][z] != BLOCK_AIR &&
                   chunk->blocks[MIN(CHUNK_SIZE, x+1)][y][z] != BLOCK_AIR &&
                   chunk->blocks[x][MAX(0, y-1)][z] != BLOCK_AIR &&
                   chunk->blocks[x][MIN(CHUNK_SIZE, y+1)][z] != BLOCK_AIR &&
                   chunk->blocks[x][y][MAX(0, z-1)] != BLOCK_AIR &&
                   chunk->blocks[x][y][MIN(CHUNK_SIZE, z+1)] != BLOCK_AIR
                )
                    continue;

                if (chunk->blocks[x][y][z] == BLOCK_AIR)
                    continue;*/

                float width = 1;
                float height = 1;
                float r = 255;
                float g = 255;
                float b = 255;
                float a = 1;

                float vertices[] =
                {
                    // positions            // colors                                // texture coords
                    0.0f,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     0.0f, 0.0f, 1.0f,   // top right
                    width,  0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     0.0f, 0.0f, 1.0f,   // bottom right
                    width,  height,  0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,     0.0f, 0.0f, 1.0f,   // bottom left
                    0.0f,   height,  0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,     0.0f, 0.0f, 1.0f,    // top left

                    // positions            // colors                                // texture c    oords
                    0.0f,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     0.0f, 0.0f, -1.0f,   // top right
                    width,  0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     0.0f, 0.0f, -1.0f,   // bottom right
                    width,  height,  width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,     0.0f, 0.0f, -1.0f,   // bottom left
                    0.0f,   height,  width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,     0.0f, 0.0f, -1.0f,    // top left

                    // positions            // colors                                // texture c    oords
                    0.0f,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     -1.0f, 0.0f, 0.0f,   // top right
                    0.0f,   height,  0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     -1.0f, 0.0f, 0.0f,   // bottom right
                    0.0f,   height,  width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,    -1.0f, 0.0f, 0.0f,   // bottom left
                    0.0f,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,    // top left

                    // positions            // colors                                // texture c    oords
                    width,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     1.0f, 0.0f, 0.0f,   // top right
                    width,   height,  0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     1.0f, 0.0f, 0.0f,   // bottom right
                    width,   height,  width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   // bottom left
                    width,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,    1.0f, 0.0f, 0.0f,    // top left

                    // positions            // colors                                // texture c    oords
                    0.0f,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     0.0f, 1.0f, 0.0f,   // top right
                    0.0f,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     0.0f, 1.0f, 0.0f,   // bottom right
                    width,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   // bottom left
                    width,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,     0.0f, 1.0f, 0.0f,    // top left

                    // positions            // colors                                // texture c    oords
                    0.0f,   height,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,    0.0f, -1.0f, 0.0f,   // top right
                    0.0f,   height,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f,   // bottom right
                    width,   height,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,  0.0f, -1.0f, 0.0f,   // bottom left
                    width,   height,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,    // top left
                };

                int yy = 0;
                int xx = 0;
                int faceid = 0;

                for (int i = 0; i < 12*24; i++)
                {
                    if (chunk->vertices == (void*)0)
                        chunk->vertices = calloc(1, sizeof(float));

                    if (xx >= 12)
                    {
                        yy += 1;
                        xx = 0; 

                        if (yy % 4 == 0)
                        {
                            faceid += 1;
                        }
                    }

                    if (faceid == 5)
                    {
                        if (chunk->blocks[x][MIN(CHUNK_SIZE, y-1)][z] != BLOCK_AIR)
                        {
                            xx += 1;
                            continue;
                        }
                    }
                    if (faceid == 4 && y > 0 && y < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[x][MAX(0, y+1)][z] != BLOCK_AIR)
                        {
                            xx += 1;
                            continue;
                        }
                    }
                    if (faceid == 3 && x > 0 && x < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[MIN(CHUNK_SIZE, x+1)][y][z] != BLOCK_AIR)
                        {
                            xx += 1;
                            continue;
                        }
                    }
                    if (faceid == 2 && x > 0 && x < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[MAX(0, x-1)][y][z] != BLOCK_AIR)
                        {
                            xx += 1;
                            continue;
                        }
                    }
                    if (faceid == 1 && z > 0 && z < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[x][y][MIN(CHUNK_SIZE, z+1)] != BLOCK_AIR)
                        {
                            xx += 1;
                            continue;
                        }
                    }
                    if (faceid == 0 && z > 0 && z < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[x][y][MAX(0, z-1)] != BLOCK_AIR)
                        {
                            xx += 1;
                            continue;
                        }
                    }

                    chunk->vertices_size += 1;
                    chunk->vertices = realloc(chunk->vertices, chunk->vertices_size * sizeof(float));
                    chunk->vertices[chunk->vertices_size-1] = vertices[i];

                    if (xx == 0)
                        chunk->vertices[chunk->vertices_size-1] += (float)x;
                    
                    if (xx == 1)
                        chunk->vertices[chunk->vertices_size-1] += (float)y;

                    if (xx == 2)
                        chunk->vertices[chunk->vertices_size-1] += (float)z;

                    xx += 1;
                }

                for (int pp = 0; pp < 6; pp++)
                {
                    chunk->indices_size += 1;
                    chunk->indices = realloc(chunk->indices, chunk->indices_size * sizeof(int));
                    chunk->indices[chunk->indices_size-1] = icount + 0;

                    chunk->indices_size += 1;
                    chunk->indices = realloc(chunk->indices, chunk->indices_size * sizeof(int));
                    chunk->indices[chunk->indices_size-1] = icount + 1;

                    chunk->indices_size += 1;
                    chunk->indices = realloc(chunk->indices, chunk->indices_size * sizeof(int));
                    chunk->indices[chunk->indices_size-1] = icount + 3;

                    chunk->indices_size += 1;
                    chunk->indices = realloc(chunk->indices, chunk->indices_size * sizeof(int));
                    chunk->indices[chunk->indices_size-1] = icount + 1; 

                    chunk->indices_size += 1;
                    chunk->indices = realloc(chunk->indices, chunk->indices_size * sizeof(int));
                    chunk->indices[chunk->indices_size-1] = icount + 2;

                    chunk->indices_size += 1;
                    chunk->indices = realloc(chunk->indices, chunk->indices_size * sizeof(int));
                    chunk->indices[chunk->indices_size-1] = icount + 3;

                    icount += 4; // wuut??
                }
                
                chunk->block_count += 1; // wait.... why?
            }
        }
    }
}

chunk_T* init_chunk(int x, int y, int z)
{
    chunk_T* chunk = calloc(1, sizeof(struct CHUNK_STRUCT));
    chunk->vertices = (void*)0;
    chunk->indices = (void*)0;
    chunk->vertices_size = 0;
    chunk->indices_size = 0;
    chunk->x = x;
    chunk->y = y;
    chunk->z = z;

    for (int y = 0; y < CHUNK_SIZE; y++)
        for (int x = 0; x < CHUNK_SIZE; x++)
            for (int z = 0; z < CHUNK_SIZE; z++)
                chunk->blocks[x][y][z] = BLOCK_COBBLE;//random_int(0, 3) == 0 ? BLOCK_AIR : BLOCK_COBBLE;


    generate_vertices(chunk);

    return chunk;
}

void chunk_draw(chunk_T* chunk)
{
    state_T* state = get_current_state();

    // TODO: Calculate and make all cube draws into one single call.

    /*for (int y = 0; y < CHUNK_SIZE; y++)
    {
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                texture_T* texture = (void*)0;

                if (chunk->blocks[x][y][z] == BLOCK_COBBLE)
                    texture = TEXTURE_COBBLE;

                if (x != 0 && y != 0 && z != 0 && x != CHUNK_SIZE - 1 && y != CHUNK_SIZE - 1 && z != CHUNK_SIZE - 1)
                if (
                   chunk->blocks[MAX(0, x-1)][y][z] != BLOCK_AIR &&
                   chunk->blocks[MIN(CHUNK_SIZE - 1, x+1)][y][z] != BLOCK_AIR &&
                   chunk->blocks[x][MIN(0, y-1)][z] != BLOCK_AIR &&
                   chunk->blocks[x][MAX(CHUNK_SIZE - 1, y+1)][z] != BLOCK_AIR &&
                   chunk->blocks[x][y][MIN(0, z-1)] != BLOCK_AIR &&
                   chunk->blocks[x][y][MAX(CHUNK_SIZE - 1, z+1)] != BLOCK_AIR
                )
                    continue;

                if (texture != (void*)0)
                    draw_cube(state, texture, x, y, z);
            }
        }
    }*/

    draw_cube(
        state,
        TEXTURE_COBBLE,
        chunk->x * CHUNK_SIZE,
        chunk->y * CHUNK_SIZE,
        chunk->z * CHUNK_SIZE,
        chunk->vertices,
        chunk->vertices_size,
        chunk->indices,
        chunk->indices_size,
        chunk->block_count
    );
}
