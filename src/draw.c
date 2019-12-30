#include "include/draw.h"
#include <coelum/draw_utils.h>


extern unsigned int SHADER_TEXTURED_SHADED;

void draw_cube(state_T* state, texture_T* texture, float x, float y, float z, float* vertices, size_t vertices_size, int* indices, size_t indices_size, size_t nr)
{
    float width = 1.0f;
    float height = 1.0f;
    float r = 255.0f;
    float g = 255.0f;
    float b = 255.0f;
    float a = 1.0f;

    /*int INDICES_DEFAULT [] =
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
    };*/

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

    /*float VERTICES_TEXTURED[] =
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
    };*/
    
    float* VERTICES_TEXTURED = vertices; 

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_TEXTURED), VERTICES_TEXTURED, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // texcoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // shiftX
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(12 * sizeof(float)));
    glEnableVertexAttribArray(4);

    // shiftY
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(13 * sizeof(float)));
    glEnableVertexAttribArray(5);

    glUniform3fv(glGetUniformLocation(SHADER_TEXTURED_SHADED, "world_pos"), 1, (float[]){ x, y, z });
    
    glUniform1i(glGetUniformLocation(SHADER_TEXTURED_SHADED, "atlas_width"), 16);
    glUniform1i(glGetUniformLocation(SHADER_TEXTURED_SHADED, "atlas_height"), 16);

    glDrawElements(GL_TRIANGLES, (6*nr), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, vertices_size);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glBindVertexArray(0);
}
