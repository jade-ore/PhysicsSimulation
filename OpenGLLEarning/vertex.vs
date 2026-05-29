#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in mat4 aTransform;
out vec3 ourColor;

uniform vec2 dimension;

void main() {
	vec4 Pos = vec4(aPos, 0.0, 1.0);
	Pos = vec4(aPos.x / dimension.x, aPos.y / dimension.y, 0.0f, 1.0f);
	gl_Position = vec4(Pos.xy, 0.0, 1.0);
	ourColor = aColor;
}