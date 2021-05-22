#pragma once
#include <string>
#include <memory>
#include <map>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Character.h"
#include "../Render/Shader.h"
#include "../Render/VertexArray.h"
#include "../Render/VertexBuffer.h"
#include "../Render/IndexBuffer.h"
#include "../Render/Renderer.h"

class Text
{
public:
	Text(const std::string fontPath, float fontScale, glm::vec3 fontColor, unsigned int height, unsigned int width = 0);
	void Draw(std::shared_ptr<Shader> shader, const std::string& text, float x, float y);
	void SetFontScale(float scale);
	void SetFontColor(glm::vec3 color);

private:
	FT_Library ftLibrary;
	FT_Face face;
	std::unique_ptr<VertexArray> VAO;
	std::map<unsigned char, std::unique_ptr<Character>> characters;
	unsigned int width;
	unsigned int height;
	float fontScale;
	glm::vec3 fontColor;

	//position, tex coords of a single character
	GLfloat vertices[4][4] = {
		{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
	};

	void LoadCharacters();
};