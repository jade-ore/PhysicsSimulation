#include "Engine.h"
#include <iostream>

bool Engine::INITIALIZED = false;

Engine::Engine(int MAJOR, int MINOR) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void Engine::initialize(int WIDTH, int HEIGHT) {
    if (INITIALIZED) return;

    window = glfwCreateWindow(WIDTH, HEIGHT, "Please Work", NULL, NULL);
    if (window == NULL) {
        std::cout << "FAILED TO CREATE GLFW WINDOW" << std::endl;
        glfwTerminate();
        throw std::runtime_error("FAILED TO CREATE GLFW WINDOW");
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("FAILED TO INITIALIZE GLAD");
        return;
    }
    INITIALIZED = true;
    is_closing = false;
    glViewport(0, 0, WIDTH, HEIGHT);
    Engine::set_background_color(glm::vec3(0.0f));
}

void Engine::set_background_color(float r, float g, float b, float a) {
    background_color = glm::vec4(r, g, b, a);
}

void Engine::set_background_color(glm::vec3 c) {
    background_color = glm::vec4(c, 1.0f);
}

void Engine::render() {
    glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    /*glfwPollEvents();
    glfwSwapBuffers(window);*/
}

bool Engine::is_key_pressed(int key) {
    return (glfwGetKey(window, key) == GLFW_PRESS);
}

bool Engine::closing() {
    if (glfwWindowShouldClose(window)) is_closing = true;
    return is_closing;
}

void Engine::close() {
    is_closing = true;
    glfwTerminate();
}

Shape::Shape(glm::vec2 in_pos, glm::vec3 in_col) {
    Position = in_pos;
    Color = in_col;
}