#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "EngineConst.h"

#include <array>
#include <cmath>
#include <stdexcept>

class Engine {
public:
	Engine(int MAJOR = 3, int MINOR = 3);
	void initialize(int WIDTH = 800, int HEIGHT = 600);
	GLFWwindow* window;
	bool closing();
	void close();
	bool is_key_pressed(int key);
	void render();
	void set_background_color(glm::vec3 c);
	void set_background_color(float r, float g, float b, float a = 1.0f);
private:
	static bool INITIALIZED;
	bool is_closing;
	glm::vec4 background_color;
};

class Shape {
public:
	Shape(glm::vec2 pos, glm::vec3 col);
	glm::vec2 Position;
	glm::vec3 Color;
};

class Triangle : public Shape {
	Triangle(std::array<glm::vec2, 3> triangle_vertices, glm::vec2 pos, glm::vec3 col);
	static std::vector<glm::vec2> vertices_buffer;
};