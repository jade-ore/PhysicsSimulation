#include "Engine.h"
#include "tuffphysics.h"

#include <random>


int main() {
	Engine engine = Engine();
	
	engine.initialize();

	RightTriangle r1(500, 300, glm::vec2(500, 200), glm::vec3(93,28,50));
	Triangle t1({
		glm::vec2(60, 200),
		glm::vec2(-10, 600),
		glm::vec2(400, 100),
		}, glm::vec2(0, 0), glm::vec3(100));

	Circle c1(300, glm::vec2(500, -50), glm::vec3(255, 255, 255));
	Rectangle rec1(300, 500, glm::vec2(-300, -100), glm::vec3(39, 128, 255));

	engine.add(&r1);
	engine.add(&t1);
	engine.add(&rec1);
	engine.PushInformation();
	
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
			dir += glm::vec2(0, -1);
		}
		if (engine.is_key_pressed(KEY_S)) {
			dir += glm::vec2(0, 1);
		}
		if (engine.is_key_pressed(KEY_A)) {
			dir += glm::vec2(1,0);
		}
		if (engine.is_key_pressed(KEY_D)) {
			dir += glm::vec2(-1,0);
		}
		if (engine.is_key_pressed(KEY_I)) {
			engine.camera.zoom(dt * 1);
		}
		if (engine.is_key_pressed(KEY_O)) {
			engine.camera.zoom(-dt * 1);
		}

		glm::vec2 cir_dir(0);

		if (engine.is_key_pressed(KEY_UP)) {
			cir_dir += glm::vec2(0, 1);
		}
		if (engine.is_key_pressed(KEY_DOWN)) {
			cir_dir += glm::vec2(0, -1);
		}
		if (engine.is_key_pressed(KEY_LEFT)) {
			cir_dir += glm::vec2(-1, 0);
		}
		if (engine.is_key_pressed(KEY_RIGHT)) {
			cir_dir += glm::vec2(1, 0);
		}

		if (dir != glm::vec2(0)) {
			engine.camera.move(glm::normalize(dir));
		}
		if (cir_dir != glm::vec2(0)) {
			c1.transform(cir_dir * dt);
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


