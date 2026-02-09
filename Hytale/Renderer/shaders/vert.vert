#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vertexColor;

uniform mat4 viewMat;

void main() {
	gl_Position = viewMat * vec4(aPos, 1.0);
	vertexColor = aColor;
}