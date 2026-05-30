#version 330 core

layout (location = 0) in vec2 aPos;

out vec3 ourColor;

uniform vec3 aColor;
uniform vec2 dimension;
uniform mat4 translation;

void main() {

	vec4 Pos = vec4(aPos, 0.0, 1.0);
	Pos = translation * Pos;

	// NDC coordinates from here
	Pos = vec4(Pos.x / dimension.x, Pos.y / dimension.y, 0.0f, 1.0f);
	gl_Position = Pos;
	ourColor = aColor;
}