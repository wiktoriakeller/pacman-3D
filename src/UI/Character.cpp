#include "Character.h"

Character::Character(glm::ivec2 size, glm::ivec2 bearing, unsigned int advance, 
	unsigned char* character) : size(size), bearing(bearing), advance(advance) {
	LoadCharacter(character);
}

Character::~Character() {
	glDeleteTextures(1, &textureID);
}

void Character::Bind(unsigned int slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Character::Unbind() const {
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::ivec2 Character::GetSize() const {
	return size;
}

glm::ivec2 Character::GetBearing() const {
	return bearing;
}

unsigned int Character::GetAdvance() const {
	return advance;
}

void Character::LoadCharacter(unsigned char* character) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size.x, size.y, 0, 
		GL_RED, GL_UNSIGNED_BYTE, (const void*) character);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}