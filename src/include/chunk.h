#ifndef VOXEL_CHUNK_H
#define VOXEL_CHUNK_H
typedef struct CHUNK_STRUCT
{
    int blocks[16][16][16];
} chunk_T;

chunk_T* init_chunk();

void chunk_draw(chunk_T* chunk);
#endif
