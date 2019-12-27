#include "include/chunk.h"
#include "include/block_type.h"
#include <coelum/current.h>
#include <coelum/textures.h>
#include <coelum/draw_utils.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <sys/param.h>


#define CHUNK_SIZE 8

extern texture_T* TEXTURE_COBBLE;
extern unsigned int SHADER_TEXTURED_SHADED;

chunk_T* init_chunk()
{
    chunk_T* chunk = calloc(1, sizeof(struct CHUNK_STRUCT));

    for (int y = 0; y < CHUNK_SIZE; y++)
        for (int x = 0; x < CHUNK_SIZE; x++)
            for (int z = 0; z < CHUNK_SIZE; z++)
                chunk->blocks[x][y][z] = BLOCK_COBBLE;

    return chunk;
}

void draw_cube(state_T* state, texture_T* texture, float x, float y, float z)
{
    float width = 1.0f;
    float height = 1.0f;
    float r = 255.0f;
    float g = 255.0f;
    float b = 255.0f;
    float a = 1.0f;

    int INDICES_DEFAULT [] =
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

    glBindVertexArray(state->VAO);
    glUseProgram(SHADER_TEXTURED_SHADED);

    glUniform1i(glGetUniformLocation(SHADER_TEXTURED_SHADED, "ourTexture"), 0); 

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->renderable_texture);

    send_projection_view_state(SHADER_TEXTURED_SHADED, state->camera->projection_view); 

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    mat4 model =
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    glm_translate(model, (vec3){x, y, z});
    send_model_state(SHADER_TEXTURED_SHADED, model);

    float VERTICES_TEXTURED[] =
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

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_TEXTURED), VERTICES_TEXTURED, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES_DEFAULT), INDICES_DEFAULT, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // texcoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glUniform3fv(glGetUniformLocation(SHADER_TEXTURED_SHADED, "world_pos"), 1, (float[]){ x, y, z });

    glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glBindVertexArray(0);
}

void chunk_draw(chunk_T* chunk)
{
    state_T* state = get_current_state();

    // TODO: Calculate and make all cube draws into one single call.

    for (int y = 0; y < CHUNK_SIZE; y++)
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
    }
}
