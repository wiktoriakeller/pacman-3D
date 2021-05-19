#version 330 core
in vec2 texCoords;

uniform sampler2D text;
uniform vec3 uTextColor;

out vec4 color;

void main() {
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
	color = vec4(uTextColor, 1.0) * sampled;
}