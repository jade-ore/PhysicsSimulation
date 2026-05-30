#include "Engine.h"
#include "tuffphysics.h"

#include <random>


int main() {
	Engine engine = Engine();
	
	engine.initialize();

	RightTriangle r1(500, 300, glm::vec2(500, 200), glm::vec3(255));
	Triangle::Push();
	engine.add(&r1);
	
	while (!engine.closing()) {

		if (engine.is_key_pressed(KEY_ESCAPE)) {
			engine.close();
		}
		
		engine.render(); // draws bg

	}
	
	engine.close();
	return 0;
}
