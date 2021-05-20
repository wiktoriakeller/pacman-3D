#version 330 core
in vec2 texCoords;

uniform sampler2D image;
uniform vec3 uColor;
uniform bool useColor;

out vec4 fragColor;

void main() {
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

	if(useColor) {
		color = vec4(uColor, 1.0);
	}
	else {
		color = texture(image, texCoords);
	
		if(color.a < 0.1)
			discard;
	}

	fragColor = color;
}