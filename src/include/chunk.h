#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H
#include <stddef.h>
#include "constants.h"
#include <cglm/cglm.h>
#include <cglm/call.h>


#define CHUNK_SIZE 16

typedef struct CHUNK_STRUCT
{
    int blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    int* indices;
    size_t indices_size;
    float* vertices;
    size_t vertices_size;
    size_t block_count;
    size_t face_count;

    int x;
    int y;
    int z;

    unsigned int VBO;
    unsigned int EBO;

    unsigned int selected;

    mat4 model;
} chunk_T;

chunk_T* init_chunk(int x, int y, int z, double heightmap[NR_CHUNKS*CHUNK_SIZE][NR_CHUNKS*CHUNK_SIZE], int* decormap);

void chunk_tick(chunk_T* chunk);

void chunk_draw(chunk_T* chunk);

void chunk_generate_vertices(chunk_T* chunk);
#endif
