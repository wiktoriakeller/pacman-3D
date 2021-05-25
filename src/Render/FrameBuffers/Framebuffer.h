#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Renderer.h"
#include "../../Game.h"

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();
	virtual void Bind();
	virtual void Unbind();
	virtual void Draw() = 0;

private:
	GLuint frameBufferID;
	std::unique_ptr<VertexArray> VAO;
};