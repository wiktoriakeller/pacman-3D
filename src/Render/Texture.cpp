#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Texture.h"

Texture::Texture(const std::string& filePath, bool flip) : textureID(0) {
	LoadFromFile(filePath, flip);
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}

void Texture::LoadFromFile(const std::string& filePath, bool flip) {
	if (textureID)
		glDeleteTextures(1, &textureID);

	GLenum format = GL_RGBA;
	
	stbi_set_flip_vertically_on_load(flip);

	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channelsNumber, 0);

	if (data) {

		if (channelsNumber == 1) {
			format = GL_RED;
		}
		else if (channelsNumber == 3) {
			format = GL_RGB;
		}
		else if (channelsNumber == 4) {
			format = GL_RGBA;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture: " << filePath << "\n";
	}
}

void Texture::Bind(GLuint slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind() const {
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}