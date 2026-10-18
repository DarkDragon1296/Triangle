// TODO: тесты на винде
// TODO: написать README
// TODO: добавить константность параметров там, где необходимо
// TODO: window size change bag
// TODO: rename namespace

#include "Renderer.hpp"
#include "Window.hpp"
#include "glad.hpp"
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


void TEST_OBJECTS(std::vector<Triangle> &batch, std::vector<Segment3D> &segs);

void TEST_OBJECTS(std::vector<Triangle> &batch, std::vector<Segment3D> &segs) {
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
    batch.push_back(tr2);
    #endif

    #if 1
    for (size_t i = 0; i < batch.size(); i++) {
        for (size_t j = i + 1; j < batch.size(); j++) {
            get_intersection_points(batch[i], batch[j], segs);
        }
    }
    #endif
}

int main(void) {
    if (glfwInit() == GLFW_FALSE) {
        std::cout << "Failed to inialialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    std::optional<Window> window = Window::create(WIDTH, HEIGHT, "triangles");
    if (!window.has_value()) {
        std::cout << "Failed to initialize window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }


    Timer timer;
    timer.get_start();

    std::vector<Triangle> batch; // default init
    std::vector<Segment3D> segs; // default init
    TEST_OBJECTS(batch, segs);
{
    Renderer renderer = {
        .triangles = {std::move(batch)},
        .segments  = {std::move(segs)},
        .camera = {},
        .shader = {vshader_path, fshader_path},
    };

    timer.get_end();
    timer.get_delta();
    std::cout << "Objs time = " << timer.delta << std::endl;




    while (!glfwWindowShouldClose(window)) {
        process_input(window, renderer.camera);
        clear_window();

        process_transforms(renderer.shader, renderer.camera);
        renderer.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}
    glfwTerminate();

    return EXIT_SUCCESS;
}
