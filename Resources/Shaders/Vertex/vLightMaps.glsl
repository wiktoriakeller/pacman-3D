#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;

out vec3 fragmentPosition;
out vec2 textureCoord;
out mat3 TBN;

void main() {
	vec4 worldSpace = uModel * vec4(aPosition, 1.0);
	gl_Position = uProjection * uView * worldSpace;
	fragmentPosition = vec3(worldSpace);
	textureCoord = aTextureCoord;

	vec3 T = normalize(uNormalMatrix * aTangent);
	vec3 B = normalize(uNormalMatrix * aBitangent);
	vec3 N = normalize(uNormalMatrix * aNormal);
	TBN = mat3(T, B, N);
}