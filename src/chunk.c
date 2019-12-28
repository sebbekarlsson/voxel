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

    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
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

                if (chunk->blocks[x][y][z] == BLOCK_AIR)
                    continue;

                float width = 1;
                float height = 1;
                float r = 255;
                float g = 255;
                float b = 255;
                float a  = 1;

                int indices [] =
                {
                    // front
                    0, 1, 3,   // first triangle
                    1, 2, 3,    // second triangle
                    
                    // back
                    4, 5, 7,   // first triangle
                    5, 6, 7,    // second triangle

                    // left
                    8, 9, 11,   // first triangle
                    9, 10, 11,    // second triangle

                    // right
                    12, 13, 15,   // first triangle
                    13, 14, 15,    // second triangle

                    // bottom
                    16, 17, 19,   // first triangle
                    17, 18, 19,    // second triangle

                    // top
                    20, 21, 23,   // first triangle
                    21, 22, 23,    // second triangle
                };

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

                for (int i = 0; i < 12*24; i++)
                {
                    if (chunk->vertices == (void*)0)
                        chunk->vertices = calloc(1, sizeof(float));

                    if (xx >= 12)
                    {
                        yy += 1;
                        xx = 0;
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

                for (int i = 0; i < 6*6; i++)
                {
                    chunk->indices_size += 1;
                    chunk->indices = realloc(chunk->indices, chunk->indices_size * sizeof(int));
                    chunk->indices[chunk->indices_size-1] = indices[i] + chunk->block_count;
                }
                
                chunk->block_count += 24; // wait.... why?
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
