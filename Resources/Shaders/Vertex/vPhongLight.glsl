#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;

out vec3 fragmentPosition;
out vec3 normal;

void main() {
	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
	fragmentPosition = vec3(uModel * vec4(aPosition, 1.0));
	normal = uNormalMatrix * aNormal;
}