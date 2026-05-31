#include "Engine.h"
#include "tuffphysics.h"

#include <random>


int main() {
	Engine engine = Engine();
	
	engine.initialize();

	RightTriangle r1(500, 300, glm::vec2(500, 200), glm::vec3(255));
	Triangle::Push();
	engine.add(&r1);

	Circle c1(300, glm::vec2(500, 0), glm::vec3(255, 255, 255));

	Circle::Push();
	
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

		float theta = 0.0f;

		glm::vec2 dir(0);

		if (engine.is_key_pressed(KEY_W)) {
			dir += glm::vec2(0,1);
		}
		if (engine.is_key_pressed(KEY_S)) {
			dir += glm::vec2(0,-1);
		}
		if (engine.is_key_pressed(KEY_A)) {
			dir += glm::vec2(-1,0);
		}
		if (engine.is_key_pressed(KEY_D)) {
			dir += glm::vec2(1,0);
		}
		if (engine.is_key_pressed(KEY_I)) {
			theta = 5;
		}
		if (engine.is_key_pressed(KEY_O)) {
			theta = -5;
		}

		if (dir != glm::vec2(0)) {
			c1.set_color(c1.Color + glm::vec3(dir.x * 50, dir.y * 50, 0));
		}
		if (theta) {
			r1.transform(glm::vec2(0), theta);
		}

		Circle::Update();
		engine.render(); // draws bg

	}
	
	engine.close();
	return 0;
}


