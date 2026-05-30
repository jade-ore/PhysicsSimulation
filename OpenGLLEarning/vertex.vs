#version 330 core

layout (location = 0) in vec2 aPos;

out vec3 ourColor;

uniform vec3 aColor;
uniform vec2 dimension;
uniform mat4 translation;
uniform mat4 camera;

void main() {

	vec4 Pos = vec4(aPos, 0.0, 1.0);
	Pos = camera * translation * Pos;

	// NDC coordinates from here
	Pos = vec4(Pos.xy / dimension, 0.0f, 1.0f);
	gl_Position = Pos;
	ourColor = aColor;
}