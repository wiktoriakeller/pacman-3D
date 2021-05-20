#version 330 core
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uProjection;

out vec2 texCoords;

void main() {
	gl_Position = uProjection * uModel * vec4(aPosition, 0.0, 1.0);
	texCoords = aTexCoords;
}