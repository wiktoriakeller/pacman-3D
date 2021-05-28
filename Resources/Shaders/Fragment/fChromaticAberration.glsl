#version 330 core

in vec2 texCoords;

uniform sampler2D image;

out vec4 fragColor;

float redOffset = 0.0012;
float greenOffset = 0.0006;
float blueOffset = -0.0006;

uniform vec2 offsetFactor;

void main() {
	vec2 direction = texCoords - offsetFactor;
	fragColor.r = texture(image, texCoords + (direction * vec2(redOffset))).r;
	fragColor.g = texture(image, texCoords + (direction * vec2(greenOffset))).g;
	fragColor.ba = texture(image, texCoords + (direction * vec2(blueOffset))).ba;
}