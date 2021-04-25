#version 330 core

uniform vec3 uLightColor;

out vec4 fragmentColor;

void main() {
	fragmentColor = vec4(uLightColor, 1.0);
}