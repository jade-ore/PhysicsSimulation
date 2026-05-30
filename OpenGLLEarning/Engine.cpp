#include "Engine.h"
#include <iostream>

bool Engine::INITIALIZED = false;

void print_mat4(glm::mat4 m) {
    for (int i = 0; i < 4; i++) {
        std::cout << m[0][i] << " " << m[1][i] << " " << m[2][i] << " " << m[3][i] << std::endl;
    }
}

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

    shader = Shader("vertex.vs", "fragshader.fs");

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

void Engine::add(Shape* shape) {
    shape_array.push_back(shape);
}

void Engine::render() {
    glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    shader.setVector2f("dimension", glm::vec2(800, 600));
   
    for (int i = 0; i < shape_array.size(); i++) {
        shader.setMatrix4f("translation", shape_array[i]->Translation);
        
        shader.setVector3f("aColor", shape_array[i]->Color);
        shape_array[i]->render();
    }

    glfwPollEvents();
    glfwSwapBuffers(window);
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

bool Triangle::INITIALIZED = false;
unsigned int Triangle::VertexVBO = 0;
unsigned int Triangle::ColorsVBO = 0;
unsigned int Triangle::VAO = 0;
std::vector<glm::vec2> Triangle::vertices_buffer;

Triangle::Triangle(std::array<glm::vec2, 3> triangle_vertices, glm::vec2 pos, glm::vec3 col) {
    // initialize the triangle VBO and VAO and stuff

    if (!INITIALIZED) {

        glGenBuffers(1, &VertexVBO);
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        // set vertex attribute for vertex
        glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));

        // turn attribute on
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        INITIALIZED = true;
    }

    Position = pos;
    Color = col;

    Translation = glm::translate(glm::mat4(1), glm::vec3(Position, 0));
    start_index = vertices_buffer.size() ;
    
    
    for (int i = 0; i < 3; i++) {
        vertices_buffer.push_back(triangle_vertices[i]);
    }
}

void Triangle::Push() {
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_buffer.size() * sizeof(glm::vec2), vertices_buffer.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::Update() {
    glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_buffer), vertices_buffer.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Triangle::render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, start_index, 3);
    

}

RightTriangle::RightTriangle(int width, int height, glm::vec2 pos, glm::vec3 col) : Triangle({
    glm::vec2(0, 0),
    glm::vec2(0, height),
    glm::vec2(width, 0)
    }, pos, col) {}


Quad::Quad(std::array<glm::vec2, 4> quad_vertices, glm::vec2 pos, glm::vec3 col) {
    t1 = new Triangle({
        quad_vertices[0],
        quad_vertices[1],
        quad_vertices[2]
        }, pos, col);

    t2 = new Triangle({
        quad_vertices[0],
        quad_vertices[2],
        quad_vertices[3]
        }, pos, col);

    Position = pos;
    Color = col;
    Translation = glm::translate(glm::mat4(1), glm::vec3(pos, 0));
}

void Quad::render() {
    t1->render();
    t2->render();
}

Rectangle::Rectangle(int width, int height, glm::vec2 pos, glm::vec3 col) : Quad({
    glm::vec2(-width / 2,  height / 2),
    glm::vec2(width / 2,  height / 2),
    glm::vec2(width / 2, -height / 2),
    glm::vec2(-width / 2, -height / 2),
    }, pos, col) {}

Square::Square(int length, glm::vec2 pos, glm::vec3 col) : Rectangle(length, length, pos, col) {}