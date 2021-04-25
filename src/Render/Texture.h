#pragma once
#include <string>
#include <memory>
#include <GL/glew.h>
#include <iostream>

class Texture
{
public:
	Texture(const std::string& filePath, bool flip = true);
	~Texture();
	void LoadFromFile(const std::string& filePath, bool flip);

	void Bind(GLuint slot) const;
	void Unbind() const;

private:
	GLuint textureID;
	int width;
	int height;
	int channelsNumber;
};