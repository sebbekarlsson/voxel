#include <coelum/main.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
#include <coelum/constants.h>
#include <coelum/scene.h>
#include <coelum/actor.h>
#include <coelum/draw_utils.h>
#include <coelum/camera.h>
#include <coelum/input.h>
#include <coelum/actor_light.h>
#include <glad/glad.h>


extern theatre_T* THEATRE;
extern mouse_state_T* MOUSE_STATE;
extern keyboard_state_T* KEYBOARD_STATE;
extern unsigned int SHADER_TEXTURED_SHADED;

texture_T* texture;

float distance;


void draw_grid(state_T* state, float x, float y, float z)
{
    float unit = 1;
    float length = unit * 200;

    // x
    for (int i = 0; i < length; i++)
    {
        draw_line(
            x,
            y,
            z + i,
            length,
            0,
            i,
            255.0f,
            0,
            0,
            state
        );
    }

    // z
    for (int i = 0; i < length; i++)
    {
        draw_line(
            x + i,
            y,
            z,
            i,
            0,
            length,
            0,
            0,
            255.0f,
            state
        );
    }

}

void draw_cube(state_T* state, float x, float y, float z)
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
        0.0f,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,   // top right
        0.0f,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,     0.0f, -1.0f, 0.0f,   // bottom right
        width,   0.0f,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f,   // bottom left
        width,   0.0f,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,     0.0f, -1.0f, 0.0f,    // top left

        // positions            // colors                                // texture c    oords
        0.0f,   height,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 0.0f,    0.0f, 1.0f, 0.0f,   // top right
        0.0f,   height,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom right
        width,   height,    width,  r / 255.0f, g / 255.0f, b / 255.0f, a,   1.0f, 1.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        width,   height,    0.0f,  r / 255.0f, g / 255.0f, b / 255.0f, a,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,    // top left
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

void custom_scene_draw(scene_T* scene)
{
    state_T* state = (state_T*) scene;
    

    camera_bind(state->camera);
    
    for (int i = 0; i < 16; i++)
        draw_cube(state, i, 0, 0);
    draw_grid(state, 0, 0, 0);

    camera_unbind(state->camera);
}

void custom_scene_tick(scene_T* scene)
{
    state_T* state = (state_T*) scene;
    


    if (KEYBOARD_STATE->keys[GLFW_KEY_W])
    {
        state->camera->x += cos(glm_rad(state->camera->ry + 90));
        state->camera->z -= sin(glm_rad(state->camera->ry + 90));
        distance += 0.3f;
    }

    if (KEYBOARD_STATE->keys[GLFW_KEY_S])
    {
        state->camera->x -= cos(glm_rad(state->camera->ry + 90));
        state->camera->z += sin(glm_rad(state->camera->ry + 90));
    }

    state->camera->y = 5 - (cos(distance) * 0.5f);

    state->camera->offset_x = state->camera->x;
    state->camera->offset_y = state->camera->y;
    state->camera->offset_z = state->camera->z;
    state->camera->rx += MOUSE_STATE->dy * 0.25f;
    state->camera->ry += MOUSE_STATE->dx * 0.25f;
    //state->camera->ry = cos(T) * 30;
}

scene_T* init_scene_main()
{
    distance = 0.0f;
    // creating a scene                          tick        draw     (2 dimensions)
    scene_T* s = scene_constructor(init_scene(), custom_scene_tick, custom_scene_draw, 3);
    s->bg_r = 154;
    s->bg_g = 55;
    s->bg_g = 200;

    state_T* state = (state_T*) s;

    state->lighting_enabled = 1;
    
    state->camera->y = 5;
    state->camera->z = 1;
    state->camera->x = 1;
    state->camera->ry = 0;

    actor_light_T* light = init_actor_light(
        0.0f, 1.0f, 2.0f,
        13.0f        
    );

    dynamic_list_append(((state_T*)s)->actors, light);
    

    return s;
} 

int main(int argc, char* argv[])
{
    coelum_init();

    texture = get_texture("cobble.png", GL_RGB);

    MOUSE_STATE->input_mode = GLFW_CURSOR_DISABLED;

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_main());

    return coelum_main(argc, argv);
 }
