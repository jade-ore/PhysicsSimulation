#version 330 core

in vec3 ourColor;
out vec4 FragColor;

void main() {
	FragColor = vec4(ourColor.r / 255, ourColor.g / 255, ourColor.b / 255, 1.0f);
}