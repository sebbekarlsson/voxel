#include "include/chunk.h"
#include "include/block_type.h"
#include "include/draw.h"
#include "include/constants.h"
#include <coelum/current.h>
#include <coelum/textures.h>
#include <coelum/draw_utils.h>
#include <coelum/utils.h>
#include <math.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <sys/param.h>
#include <string.h>


extern texture_T* TEXTURE_ATLAS;
extern chunk_T* chunks[NR_CHUNKS][NR_CHUNKS_Y][NR_CHUNKS];

chunk_T* init_chunk(int x, int y, int z, double heightmap[NR_CHUNKS*CHUNK_SIZE][NR_CHUNKS*CHUNK_SIZE], int* decormap)
{
    chunk_T* chunk = calloc(1, sizeof(struct CHUNK_STRUCT));
    chunk->vertices = (void*)0;
    chunk->indices = (void*)0;
    chunk->vertices_size = 0;
    chunk->indices_size = 0;
    chunk->block_count = 0;
    chunk->face_count = 0;
    chunk->x = x;
    chunk->y = y;
    chunk->z = z;
    chunk->selected = 0;

    glGenBuffers(1, &chunk->VBO);

    glGenBuffers(1, &chunk->EBO);

    mat4 model =
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    memcpy(chunk->model, model, sizeof(model));
    
    glm_translate(chunk->model, (vec3){chunk->x * CHUNK_SIZE, chunk->y * CHUNK_SIZE, chunk->z * CHUNK_SIZE});

    int height = CHUNK_SIZE / 2;

    for (int cx = 0; cx < CHUNK_SIZE; cx++)
    {
        for (int cz = 0; cz < CHUNK_SIZE; cz++)
        {
            for (int cy = 0; cy < CHUNK_SIZE; cy++)
            {
                chunk->blocks[cx][cy][cz] = BLOCK_AIR;
            }
        }
    }

    for (int cx = 0; cx < CHUNK_SIZE; cx++)
    {
        for (int cz = 0; cz < CHUNK_SIZE; cz++)
        {
            
            int rx = (x*CHUNK_SIZE)+cx; 
            int rz = (z*CHUNK_SIZE)+cz; 
            height = MAX(0,  CHUNK_SIZE * heightmap[rx][rz]);

            for (int cy = chunk->y * CHUNK_SIZE; cy < height; cy++)
            {
                int placey = cy % 16;
                int decor = decormap[(rz * (NR_CHUNKS*CHUNK_SIZE) * (NR_CHUNKS_Y*CHUNK_SIZE)) + (cy * (NR_CHUNKS*CHUNK_SIZE)) + rx];

                chunk->blocks[cx][placey][cz] = decor != BLOCK_VOID ? decor : BLOCK_GRASS;

                if (placey >= CHUNK_SIZE)
                    break;
            }
        } 
    }

    for (int cx = 0; cx < CHUNK_SIZE; cx++)
    {
        int rx = (x*CHUNK_SIZE)+cx;

        for (int cz = 0; cz < CHUNK_SIZE; cz++)
        {
            
            int rz = (z*CHUNK_SIZE)+cz; 

            for (int cy = 0; cy < CHUNK_SIZE; cy++)
            {
                int ry = (y*CHUNK_SIZE)+cy; 
                int decor = decormap[(rz * (NR_CHUNKS*CHUNK_SIZE) * (NR_CHUNKS_Y*CHUNK_SIZE)) + (ry * (NR_CHUNKS*CHUNK_SIZE)) + rx];
                
                if (decor != BLOCK_VOID)
                    chunk->blocks[cx][cy][cz] = decor;
            }
        }
    }


    chunk_generate_vertices(chunk);

    return chunk;
}

void chunk_tick(chunk_T* chunk)
{
}

void chunk_draw(chunk_T* chunk)
{
    state_T* state = get_current_state();

    draw_cube(
        state,
        TEXTURE_ATLAS,
        chunk->x * CHUNK_SIZE,
        chunk->y * CHUNK_SIZE,
        chunk->z * CHUNK_SIZE,
        chunk->vertices,
        chunk->vertices_size,
        chunk->indices,
        chunk->indices_size,
        chunk->face_count,
        chunk->VBO,
        chunk->EBO,
        chunk->model
    );

    if (chunk->selected)
        draw_3D_axis(
            chunk->x * CHUNK_SIZE,
            (chunk->y * CHUNK_SIZE),
            chunk->z * CHUNK_SIZE,
            CHUNK_SIZE,
            CHUNK_SIZE,
            CHUNK_SIZE,
            state
        );

    chunk->selected = 0;
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
    chunk->face_count = 0;

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
                    case BLOCK_LOG: shiftX = 4; shiftY = 1; break;
                    case BLOCK_LEAF: shiftX = 5; shiftY = 3; break;
                    case BLOCK_PLANKS: shiftX = 4; shiftY = 0; break;
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
                int skips[6];
                skips[0] = 0;

                for (int i = 0; i < 14*24; i++)
                {
                    if (xx >= 14)
                    {
                        yy += 1;
                        xx = 0; 

                        if (yy % 4 == 0)
                        {
                            faceid += 1;
                            skips[faceid] = 0;
                        }
                    }

                    if (faceid == 4)
                    {
                        if (chunk->blocks[x][MAX(0, y-1)][z] != BLOCK_AIR)
                        {
                            xx += 1;
                            skips[faceid] = 1;
                            continue;
                        }
                    }
                    else if (faceid == 5 && y >= 0 && y < CHUNK_SIZE-1) // top
                    {
                        if (chunk->blocks[x][MIN(CHUNK_SIZE-1, y+1)][z] != BLOCK_AIR)
                        {
                            xx += 1;
                            skips[faceid] = 1;
                            continue;
                        }
                    }
                    else if (faceid == 3 && x >= 0 && x < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[MIN(CHUNK_SIZE, x+1)][y][z] != BLOCK_AIR)
                        {
                            xx += 1;
                            skips[faceid] = 1;
                            continue;
                        }
                    }
                    else if (faceid == 2 && x > 0 && x <= CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[MAX(0, x-1)][y][z] != BLOCK_AIR)
                        {
                            xx += 1;
                            skips[faceid] = 1;
                            continue;
                        }
                    }
                    else if (faceid == 1 && z >= 0 && z < CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[x][y][MIN(CHUNK_SIZE, z+1)] != BLOCK_AIR)
                        {
                            xx += 1;
                            skips[faceid] = 1;
                            continue;
                        }
                    }
                    else if (faceid == 0 && z > 0 && z <= CHUNK_SIZE - 1)
                    {
                        if (chunk->blocks[x][y][MAX(0, z-1)] != BLOCK_AIR)
                        {
                            xx += 1;
                            skips[faceid] = 1;
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

                int skipped = 0;
                skipped += skips[0];
                skipped += skips[1];
                skipped += skips[2];
                skipped += skips[3];
                skipped += skips[4];
                skipped += skips[5];
                int nrfaces = 6 - skipped;

                for (int pp = 0; pp < nrfaces; pp++)
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

                    icount += 4;
                }
                
                chunk->face_count += nrfaces;
                chunk->block_count += 1; // wait.... why?
            }
        }
    }
}
