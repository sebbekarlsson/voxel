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
#include "include/chunk.h"


extern theatre_T* THEATRE;
extern mouse_state_T* MOUSE_STATE;
extern keyboard_state_T* KEYBOARD_STATE;

texture_T* TEXTURE_COBBLE;

float distance;

#define NR_CHUNKS 6

chunk_T* chunks[NR_CHUNKS][1][NR_CHUNKS];



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

void custom_scene_draw(scene_T* scene)
{
    state_T* state = (state_T*) scene;
    
    camera_bind(state->camera);
    
    draw_grid(state, 0, 0, 0);

    for (int y = 0; y < 1; y++)
        for (int x = 0; x < NR_CHUNKS; x++)
            for (int z = 0; z < NR_CHUNKS; z++)
                chunk_draw(chunks[x][y][z]);


    camera_unbind(state->camera);
}

void custom_scene_tick(scene_T* scene)
{
    state_T* state = (state_T*) scene;

    float wspeed = 0.2f;
    
    if (KEYBOARD_STATE->keys[GLFW_KEY_W])
    {
        state->camera->x += cos(glm_rad(state->camera->ry + 90)) * wspeed;
        state->camera->z -= sin(glm_rad(state->camera->ry + 90)) * wspeed;
        distance += 0.3f;
    }

    if (KEYBOARD_STATE->keys[GLFW_KEY_S])
    {
        state->camera->x -= cos(glm_rad(state->camera->ry + 90)) * wspeed;
        state->camera->z += sin(glm_rad(state->camera->ry + 90)) * wspeed;
    }

    state->camera->y = 1 - (cos(distance) * 0.2f);

    state->camera->offset_x = state->camera->x;
    state->camera->offset_y = state->camera->y;
    state->camera->offset_z = state->camera->z;
    state->camera->rx += MOUSE_STATE->dy * 0.25f;
    state->camera->ry += MOUSE_STATE->dx * 0.25f;
}

scene_T* init_scene_main()
{
    distance = 0.0f;

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
        0.0f, 2.0f, 2.0f,
        13.0f        
    );

    dynamic_list_append(((state_T*)s)->actors, light);

    for (int y = 0; y < 1; y++)
        for (int x = 0; x < NR_CHUNKS; x++)
            for (int z = 0; z < NR_CHUNKS; z++)
                chunks[x][y][z] = init_chunk(x, y, z);
    
    return s;
} 

int main(int argc, char* argv[])
{
    coelum_init();

    TEXTURE_COBBLE = get_texture("cobble.png", GL_RGB);

    MOUSE_STATE->input_mode = GLFW_CURSOR_DISABLED;

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_main());

    return coelum_main(argc, argv);
 }
