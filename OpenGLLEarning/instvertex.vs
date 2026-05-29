#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in mat4 transform;

uniform mat4 camera;

out vec3 ourColor;
void main() {
	
	gl_Position = camera * transform * vec4(aPos, 0.0f, 1.0f);
	ourColor = aColor;
}