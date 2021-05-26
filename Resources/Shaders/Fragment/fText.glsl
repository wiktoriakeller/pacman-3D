#version 330 core
in vec2 texCoords;

uniform sampler2D image;
uniform vec4 uTextColor;

out vec4 color;

void main() {
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(image, texCoords).r);
	color = uTextColor * sampled;
}