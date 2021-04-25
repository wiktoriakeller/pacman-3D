#pragma once
#include <string>
#include <memory>
#include <GL/glew.h>
#include <iostream>

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();
	void LoadFromFile(const std::string& filePath);

	void Bind(GLuint slot) const;
	void Unbind() const;

private:
	GLuint textureID;
	int width;
	int height;
	int channelsNumber;
};