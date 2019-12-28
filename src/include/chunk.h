#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H
#include <stddef.h>


typedef struct CHUNK_STRUCT
{
    int blocks[16][16][16];
    int* indices;
    size_t indices_size;
    float* vertices;
    size_t vertices_size;
    size_t block_count;

    int x;
    int y;
    int z;
} chunk_T;

chunk_T* init_chunk();

void chunk_draw(chunk_T* chunk);
#endif
