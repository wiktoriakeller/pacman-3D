#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>

class Character
{
public:
	Character(glm::ivec2 size, glm::ivec2 bearing, unsigned int advance, unsigned char* character);
	~Character();
	void Bind(unsigned int slot) const;
	void Unbind() const;
	glm::ivec2 GetSize() const;
	glm::ivec2 GetBearing() const;
	unsigned int GetAdvance() const;

private:
	GLuint textureID;
	glm::ivec2 size; //size of glyph
	glm::ivec2 bearing; //offset from vaseline to left / top of glyph
	unsigned int advance; //offset to advance to next glyph;

	void LoadCharacter(unsigned char* character);
};