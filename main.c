#include <coelum/main.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
#include <coelum/constants.h>
#include <coelum/scene.h>
#include <coelum/actor.h>
#include <coelum/draw_utils.h>
#include <coelum/camera.h>
#include <coelum/input.h>
#include <glad/glad.h>


extern theatre_T* THEATRE;
extern mouse_state_T* MOUSE_STATE;
extern keyboard_state_T* KEYBOARD_STATE;

float distance;


void draw_grid(state_T* state, float x, float y, float z)
{
    float unit = 1;
    float length = unit * 200;

    // x
    for (int i = 0; i < length; i++)
    {
        if (i % 2 == 0)
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
    }

    // z
    for (int i = 0; i < length; i++)
    {
        if (i % 2 == 0)
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

}

void draw_cube(state_T* state, float x, float y, float z)
{
}

void custom_scene_draw(scene_T* scene)
{
    state_T* state = (state_T*) scene;

    camera_bind(state->camera);

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
    
    state->camera->y = 5;
    state->camera->z = 1;
    state->camera->x = 1;
    state->camera->ry = 0;

    return s;
} 

int main(int argc, char* argv[])
{
    coelum_init();

    MOUSE_STATE->input_mode = GLFW_CURSOR_DISABLED;

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_main());

    return coelum_main(argc, argv);
 }
