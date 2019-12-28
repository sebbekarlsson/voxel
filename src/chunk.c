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


extern texture_T* TEXTURE_COBBLE;

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

    int height = CHUNK_SIZE / 2;

    for (int cx = 0; cx < CHUNK_SIZE; cx++)
    {
        for (int cz = 0; cz < CHUNK_SIZE; cz++)
        {
            for (int cy = 0; cy < MIN(CHUNK_SIZE-2, MAX(2, height)); cy++)
            {
                chunk->blocks[cx][cy][cz] = cy >= height - 1 ? BLOCK_GRASS : BLOCK_DIRT;
            }

            if (random_int(0, 64) == 0)
            {
                int radius = random_int(3, 7);
                int h = height;
                
                for (int yy = height; yy < height + radius; yy++)
                {
                    for(int zz=-radius; zz<=radius; zz++)
                    {
                        for(int xx=-radius; xx<=radius; xx++)
                        {
                            if (floor(vec2_distance(cx + xx, cz + zz, cx, cz)) < radius)
                            chunk->blocks[MIN(CHUNK_SIZE-1,MAX(0, cx+xx))][yy][MIN(CHUNK_SIZE-1, MAX(0, cz+zz))] = yy < ((height + radius)-random_int(3, 4)) ? BLOCK_DIRT : BLOCK_GRASS;
                        }
                    }

                    radius -= 1;
                }
            }
        } 
    }


    chunk_generate_vertices(chunk);

    return chunk;
}

void chunk_draw(chunk_T* chunk)
{
    state_T* state = get_current_state();

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

void chunk_generate_vertices(chunk_T* chunk)
{
    if (chunk->vertices != (void*)0)
        free(chunk->vertices);

    if (chunk->indices != (void*)0)
        free(chunk->indices);

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
                if (x != 0 && y != 0 && z != 0 && x != CHUNK_SIZE - 1 && y != CHUNK_SIZE - 1 && z != CHUNK_SIZE - 1)
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
                    continue;

                float width = 1;
                float height = 1;
                float r = 255;
                float g = 255;
                float b = 255;
                float a = 1;

                int shiftX = 0;
                int shiftY = 0;

                switch (chunk->blocks[x][y][z])
                {
                    case BLOCK_GRASS: shiftX = 0; break;
                    case BLOCK_COBBLE: shiftX = 0; shiftY = 1; break;
                    case BLOCK_DIRT: shiftX = 2; break;
                }

                float vertices[] =
                {
                    // positions            // colors                                // texture coords
                    0.0f,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     0.0f, 0.0f, 1.0f,  shiftX, shiftY, // top right
                    width,  0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     0.0f, 0.0f, 1.0f,  shiftX, shiftY, // bottom right
                    width,  height,  0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,     0.0f, 0.0f, 1.0f,  shiftX, shiftY, // bottom left
                    0.0f,   height,  0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,     0.0f, 0.0f, 1.0f,  shiftX, shiftY,  // top left

                    // positions            // colors                                // texture c    oords
                    0.0f,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     0.0f, 0.0f, -1.0f, shiftX, shiftY,   // top right
                    width,  0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     0.0f, 0.0f, -1.0f, shiftX, shiftY,   // bottom right
                    width,  height,  width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,     0.0f, 0.0f, -1.0f, shiftX, shiftY,   // bottom left
                    0.0f,   height,  width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,     0.0f, 0.0f, -1.0f, shiftX, shiftY,    // top left

                    // positions            // colors                                // texture c    oords
                    0.0f,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     -1.0f, 0.0f, 0.0f, shiftX, shiftY, // top right
                    0.0f,   height,  0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     -1.0f, 0.0f, 0.0f, shiftX, shiftY, // bottom right
                    0.0f,   height,  width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,    -1.0f, 0.0f, 0.0f, shiftX, shiftY, // bottom left
                    0.0f,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,    -1.0f, 0.0f, 0.0f, shiftX, shiftY,  // top left

                    // positions            // colors                                // texture c    oords
                    width,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     1.0f, 0.0f, 0.0f, shiftX, shiftY, // top right
                    width,   height,  0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     1.0f, 0.0f, 0.0f, shiftX, shiftY, // bottom right
                    width,   height,  width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,    1.0f, 0.0f, 0.0f, shiftX, shiftY, // bottom left
                    width,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,    1.0f, 0.0f, 0.0f, shiftX, shiftY,  // top left

                    // positions            // colors                                // texture c    oords
                    0.0f,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     0.0f, 1.0f, 0.0f,  shiftX, shiftY, // top right
                    0.0f,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     0.0f, 1.0f, 0.0f, shiftX, shiftY, // bottom right
                    width,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,   0.0f, 1.0f, 0.0f,  shiftX, shiftY, // bottom left
                    width,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,     0.0f, 1.0f, 0.0f, shiftX, shiftY,  // top left

                    // positions            // colors                                // texture c    oords
                    0.0f,   height,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,    0.0f, -1.0f, 0.0f, shiftX, shiftY,  // top right
                    0.0f,   height,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f, shiftX, shiftY,  // bottom right
                    width,   height,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,  0.0f, -1.0f, 0.0f, shiftX, shiftY,  // bottom left
                    width,   height,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f, shiftX, shiftY,    // top left
                };

                int yy = 0;
                int xx = 0;
                int faceid = 0;

                for (int i = 0; i < 14*24; i++)
                {
                    if (xx >= 14)
                    {
                        yy += 1;
                        xx = 0; 

                        if (yy % 4 == 0)
                        {
                            faceid += 1;
                        }
                    }

                    if (faceid == 4)
                    {
                        if (chunk->blocks[x][MAX(CHUNK_SIZE, y+1)][z] != BLOCK_AIR && y < CHUNK_SIZE - 1)
                        {
                            xx += 1;
                            continue;
                        }
                    }
                    else if (faceid == 4 && y > 0 && y < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[x][MAX(0, y-1)][z] != BLOCK_AIR && chunk->blocks[x][MIN(CHUNK_SIZE, y+1)][z] != BLOCK_AIR)
                        {
                            //xx += 1;
                            //continue;
                        }
                    }
                    else if (faceid == 3 && x > 0 && x < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[MIN(CHUNK_SIZE, x+1)][y][z] != BLOCK_AIR)
                        {
                            xx += 1;
                            continue;
                        }
                    }
                    else if (faceid == 2 && x > 0 && x < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[MAX(0, x-1)][y][z] != BLOCK_AIR)
                        {
                            xx += 1;
                            continue;
                        }
                    }
                    else if (faceid == 1 && z > 0 && z < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[x][y][MIN(CHUNK_SIZE, z+1)] != BLOCK_AIR)
                        {
                            xx += 1;
                            continue;
                        }
                    }
                    else if (faceid == 0 && z > 0 && z < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[x][y][MAX(0, z-1)] != BLOCK_AIR)
                        {
                            xx += 1;
                            continue;
                        }
                    }

                    
                    chunk->vertices_size += 1;

                    if (chunk->vertices == (void*)0)
                        chunk->vertices = calloc(chunk->vertices_size, sizeof(float));
                    else
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
