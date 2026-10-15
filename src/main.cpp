// TODO: тесты на винде
// TODO: написать README
// TODO: добавить константность параметров там, где необходимо
// TODO: window size change bag
// TODO: rename namespace

#include "glad.hpp"
#include "shader.hpp"
#include "render.hpp"
#include "camera.hpp"
#include "test_generator.hpp"
#include "RenderableBatch.hpp"
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

const char *vshader_path = "src/shaders/vertex_shader.vs";
const char *fshader_path = "src/shaders/fragment_shader.fs";

using namespace TOP_LEVEL_NAMESPACE;

void TEST_OBJECTS(std::vector<Triangle> &batch) {
    Triangle tr1 = {{
        {0.0f, 0.0f, 0.0f},
        {2.0f, 2.0f, 0.0f},
        {0.0f, 2.0f, 0.0f}
    }};

    Triangle tr2 = {{
        {0.0f, 0.0f,  1.0f},
        {2.0f, 2.0f,  0.0f},
        {0.0f, 2.0f, -1.0f}
    }};

    #if 1
    generate_triangle_test(batch);
    #endif

    #if 0
    batch.push_back(tr1);
    batch.triangles.push_back(tr2);
    #endif

    #if 0
    for (size_t i = 0; i < objs.triangles.size(); i++) {
        for (size_t j = i + 1; j < objs.triangles.size(); j++) {
            get_intersection_points(objs.triangles[i], objs.triangles[j], objs);
        }
    }
    #endif
}

int main(void) {
    GLFWwindow *window = create_window();

    Shader shader = Shader(vshader_path, fshader_path);

    Timer timer;
    timer.get_start();

    std::vector<Triangle> batch; // default init
    TEST_OBJECTS(batch);
{
    RenderableBatch<Triangle> triangles{std::move(batch)}; // TODO: (*) combine objs and vobjs (WIP!!!)

    timer.get_end();
    timer.get_delta();
    std::cout << "Objs time = " << timer.delta << std::endl;
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

}
    glDeleteProgram(shader.id);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
