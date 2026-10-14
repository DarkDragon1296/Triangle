// TODO: тесты на винде
// TODO: написать README
// TODO: добавить константность параметров там, где необходимо
// TODO: window size change bag
// TODO: rename namespace

#include "glad.hpp"
#include "shader.hpp"
#include "render.hpp"
#include "camera.hpp"
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

const char *vshader_path = "src/shaders/vertex_shader.vs";
const char *fshader_path = "src/shaders/fragment_shader.fs";

using namespace TOP_LEVEL_NAMESPACE;

int main(void) {
    GLFWwindow *window = create_window();
    Shader shader = Shader(vshader_path, fshader_path);
    Timer timer;
    timer.get_start();
    RenderObjects objs  = get_objects(); // TODO: (*) combine objs and vobjs
    timer.get_end();
    timer.get_delta();
    std::cout << "Objs time = " << timer.delta << std::endl;
    VertexObjects vobjs = get_vertex_objects(objs);
    Camera camera;

    setup_render();

    while (!glfwWindowShouldClose(window)) {
        process_input(window, camera);
        clear_window();

        process_transforms(shader, camera);
        draw_objects(objs, vobjs, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    terminate_processes(vobjs.triangles);
    terminate_processes(vobjs.segments);
    glDeleteProgram(shader.id);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
