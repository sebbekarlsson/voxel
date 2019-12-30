#ifndef VOXEL_DRAW_H
#define VOXEL_DRAW_H
#include <coelum/textures.h>
#include <coelum/state.h>
#include <stddef.h>
#include <glad/glad.h>


void draw_cube(state_T* state, texture_T* texture, float x, float y, float z, float* vertices, size_t vertices_size, int* indices, size_t indices_size, size_t nr, unsigned int VBO, unsigned int EBO, mat4 model);
#endif
