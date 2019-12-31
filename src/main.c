#include <coelum/main.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
#include <coelum/constants.h>
#include <coelum/scene.h>
#include <coelum/actor.h>
#include <coelum/camera.h>
#include <coelum/input.h>
#include <coelum/utils.h>
#include <glad/glad.h>
#include "include/chunk.h"
#include "include/scene_world.h"


extern mouse_state_T* MOUSE_STATE;
extern volatile int SEED;
extern theatre_T* THEATRE;

texture_T* TEXTURE_COBBLE;


int main(int argc, char* argv[])
{
    coelum_init();

    init_random();

    SEED = random_int(16, 100000);

    TEXTURE_COBBLE = get_texture("res/sheet.png", GL_RGBA);

    MOUSE_STATE->input_mode = GLFW_CURSOR_DISABLED;

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_world());

    return coelum_main(argc, argv);
 }
