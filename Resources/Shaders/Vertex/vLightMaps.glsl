#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexturePosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 fragmentPosition;
out vec3 normal;
out vec2 texturePosition;

void main() {
	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
	fragmentPosition = vec3(uModel * vec4(aPosition, 1.0));
	normal = mat3(transpose(inverse(uModel))) * aNormal;
	texturePosition = aTexturePosition;
}