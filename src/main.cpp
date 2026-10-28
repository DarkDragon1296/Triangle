// TODO: тесты на винде
// TODO: написать README
// TODO: добавить константность параметров там, где необходимо
// TODO: window size change bag
// TODO: 405 elements in spiral test div by zero

#include "Renderer.hpp"
#include "Window.hpp"
#include "Timer.hpp"
#include "test_generator.hpp"
#include <iostream>

const char *vshader_path = "src/shaders/vertex_shader.vs";
const char *fshader_path = "src/shaders/fragment_shader.fs";

using namespace render;
using namespace test_gen;
using namespace gmt;
using namespace utils;

int run_app();

int main(void) {
    if (glfwInit() == GLFW_FALSE) {
        std::cout << "Failed to inialialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    int exit_code = run_app();
    glfwTerminate();
    return exit_code;
}

int run_app() {
    Timer timer;
    timer.get_start();

    ObjsData objs{};
    TEST_OBJECTS(objs);

    std::optional<Window> opt = Window::create(WIDTH, HEIGHT, "triangles");
    if (!opt.has_value()) {
        std::cout << "Failed to initialize window" << std::endl;
        return EXIT_FAILURE;
    }
    Window &window = *opt;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    Renderer renderer = {
        std::move(objs.tr_data.move_tr()),
        std::move(objs.int_seg),
        Camera{},
        Shader{vshader_path, fshader_path},
    };

    timer.get_end();
    timer.get_delta();
    std::cout << "Objs time = " << timer.delta << std::endl;

    while (!glfwWindowShouldClose(window.get())) {
        process_input(window, renderer.camera);

        renderer.clear();
        renderer.draw();

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}
