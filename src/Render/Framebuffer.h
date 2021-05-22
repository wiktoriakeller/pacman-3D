#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "../Game.h"

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();
	void Bind() const;
	void Unbind() const;
	void Draw() const;

private:
	GLuint frameBufferID;
	GLuint colorTextureID;
	GLuint renderBufferID;
	std::unique_ptr<VertexArray> VAO;
};