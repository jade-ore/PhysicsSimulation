#include "Engine.h"
#include "tuffphysics.h"

#include <random>


int main() {
	Engine engine = Engine();
	
	engine.initialize();

	RightTriangle r1(500, 300, glm::vec2(500, 200), glm::vec3(255));
	Triangle::Push();
	engine.add(&r1);
	
	float dt;
	float last_time = 0.0f;
	float current_time;

	while (!engine.closing()) {

		current_time = glfwGetTime();
		dt = current_time - last_time;
		last_time = current_time;

		if (engine.is_key_pressed(KEY_ESCAPE)) {
			engine.close();
		}

		glm::vec2 dir(0);
		if (engine.is_key_pressed(KEY_W)) {
			dir += glm::vec2(0,-1);
		}
		if (engine.is_key_pressed(KEY_S)) {
			dir += glm::vec2(0,1);
		}
		if (engine.is_key_pressed(KEY_A)) {
			dir += glm::vec2(1,0);
		}
		if (engine.is_key_pressed(KEY_D)) {
			dir += glm::vec2(-1,0);
		}
		if (engine.is_key_pressed(KEY_I)) {
			engine.camera.zoom(dt);
		}
		if (engine.is_key_pressed(KEY_O)) {
			engine.camera.zoom(-dt);
		}
		if (dir != glm::vec2(0)) {
			engine.camera.move(glm::normalize(dir));
		}

		engine.render(); // draws bg

	}
	
	engine.close();
	return 0;
}
