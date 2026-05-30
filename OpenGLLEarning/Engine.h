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

class Shape {
public:
	Shape() {};
	Shape(glm::vec2 pos, glm::vec3 col);
	glm::vec2 Position;
	glm::vec3 Color;
	glm::mat4 Translation;
	virtual void render() { };
};

class Camera {
public:

	Camera();
	glm::mat4 get_matrix() {
		return glm::scale(matrix, glm::vec3(current_scale, current_scale, 1));
	}
	void zoom(float amount);
	void move(glm::vec2 d);
	
private:
	float current_scale;
	glm::mat4 matrix;
};


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
	void add(Shape* shape);
	Shader shader;
	Camera camera;
private:
	static bool INITIALIZED;
	bool is_closing;
	glm::vec4 background_color;
	std::vector<Shape*> shape_array;
};

class Triangle : public Shape {
public:	
	Triangle() {};
	Triangle(std::array<glm::vec2, 3> triangle_vertices, glm::vec2 pos, glm::vec3 col);
	static std::vector<glm::vec2> vertices_buffer;
	void render();
	static void Push();
	static void Update();
private:
	int start_index;
	static bool INITIALIZED;
	static unsigned int VertexVBO;
	static unsigned int ColorsVBO;
	static unsigned int VAO;
};

class RightTriangle : public Triangle {
public:
	RightTriangle(int width, int height, glm::vec2 pos, glm::vec3 col);
};

class Quad : public Shape {
public:
	Quad(std::array<glm::vec2, 4> quad_vertices, glm::vec2 pos, glm::vec3 col);
	Triangle* t1;
	Triangle* t2;
	void render();
};

class Rectangle : public Quad {
public:
	Rectangle(int width, int height, glm::vec2 pos, glm::vec3 col);
};

class Square : public Rectangle {
public:
	Square(int length, glm::vec2 pos, glm::vec3 col);
};