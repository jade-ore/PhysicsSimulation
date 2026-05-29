#include "Engine.h"
#include "tuffphysics.h"

#include <random>


int main() {
	Engine engine = Engine();
	
	engine.initialize();


	float avertices[] = {
		 0.5f, -0.5f,
		-0.5f, -0.5f,
		 0.0f,  0.5f
	};

	float vertices[] = {
		 400, -300,
		 0,   -300,
		 0,    400
	};

	float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	Shader shader("vertex.vs", "fragshader.fs");
	unsigned int VBO;
	unsigned int ColorVBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &ColorVBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	  
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(1);

	
	shader.use();
	shader.setVector2f("dimension", glm::vec2(800, 600));
	
	while (!engine.closing()) {

		if (engine.is_key_pressed(KEY_ESCAPE)) {
			engine.close();
		}
		
		engine.render(); // draws bg

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwPollEvents();
		glfwSwapBuffers(engine.window);
	}
	
	engine.close();
	return 0;
}
