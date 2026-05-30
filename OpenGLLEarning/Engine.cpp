#include "Engine.h"
#include <iostream>

/*
NEXT GOALS

Circle

Transformation

*/

bool Engine::INITIALIZED = false;

void print_mat4(glm::mat4 m) {
    for (int i = 0; i < 4; i++) {
        std::cout << m[0][i] << " " << m[1][i] << " " << m[2][i] << " " << m[3][i] << std::endl;
    }
}

void print_vec2(glm::vec2 v) {
    std::cout << v.x << ", " << v.y << std::endl;
}

void print_vec4(glm::vec4 v ) {
    std::cout << v.x << ", " << v.y << std::endl;
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
    instanced_shader = Shader("instvertex.vs", "fragshader.fs");
    glm::mat4 camera(1);
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

    if (Circle::isInitialized()) {
        instanced_shader.use();
        instanced_shader.setVector2f("dimension", glm::vec2(800, 600));
        instanced_shader.setMatrix4f("camera", camera.get_matrix());
        Circle::Render();
    }

    shader.use();

    shader.setVector2f("dimension", glm::vec2(800, 600));
    shader.setMatrix4f("camera", camera.get_matrix());
   
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
    Translation = glm::mat4(1);
    Translation = glm::translate(Translation, glm::vec3(Position, 0));
    start_index = vertices_buffer.size();
    
    
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

Camera::Camera() {
    matrix = glm::mat4(1);
    current_scale = 1.0f;
}

void Camera::move(glm::vec2 d) {
    matrix = glm::translate(matrix, glm::vec3(d, 0));
}

void Camera::zoom(float amount) {
    current_scale += amount;
}

bool Circle::BUFFERS_INITIALIZED = false;
unsigned int Circle::amount;
unsigned int Circle::ColorVBO;
unsigned int Circle::TransformVBO;
unsigned int Circle::VertexVBO;
unsigned int Circle::VAO;
std::vector<glm::vec3> Circle::colors;
std::vector<glm::mat4> Circle::transforms;

Circle::Circle(int radius, glm::vec2 pos, glm::vec3 col) {

    Translation = glm::mat4(1);

    if (!BUFFERS_INITIALIZED) {
        // initialize VBO and VAO
        glGenBuffers(1, &VertexVBO);
        glGenBuffers(1, &ColorVBO);
        glGenBuffers(1, &TransformVBO);

        glGenVertexArrays(1, &VAO);

        // bind VAO and vertex VBO
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);

        // generate points
        glm::vec2 circle_points[362];
        circle_points[0] = glm::vec2(0, 0);

        float PI = 3.141592653589793238;

        for (int i = 1; i < 361; i++) {
            circle_points[i] = glm::vec2(cos(i / 180.0f * PI), sin(i / 180.0f * PI));
        }
        
        // because floating point numbers operations can't be exact, sin(360) will equal a super small number that won't be rendered which is why you go over
        circle_points[361] = glm::vec2(cos(1 / 180.0f * PI), sin(1 / 180.0f * PI));

        // buffer data into the vertex VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(circle_points), circle_points, GL_STATIC_DRAW);

        // enable vertex attribute pointer for vertex VBO
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(0));
        glEnableVertexAttribArray(0);

        // bind color VBO
        glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);

        // enable the vertex attribute pointer for color VBO
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
        glEnableVertexAttribArray(1);

        // enable vertex attribute pointers for transform VBO
        glBindBuffer(GL_ARRAY_BUFFER, TransformVBO);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);

        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        BUFFERS_INITIALIZED = true;
    }
    Color = col;
    Position = pos;
    start_index = transforms.size();

    Translation = glm::translate(Translation, glm::vec3(pos, 0));
    Translation = glm::scale(Translation, glm::vec3(radius, radius, 0));

    print_mat4(Translation);
    colors.push_back(col);
    transforms.push_back(Translation);
    amount += 1;
}

void Circle::Push() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, TransformVBO);
    glBufferData(GL_ARRAY_BUFFER, transforms.size() * 4 * sizeof(glm::vec4), transforms.data(), GL_STATIC_DRAW);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Circle::Render() {
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 362, amount);
}

bool Circle::isInitialized() {
    return BUFFERS_INITIALIZED;
}