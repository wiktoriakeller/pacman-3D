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
	Framebuffer(GLsizei samples);
	~Framebuffer();
	virtual void Bind();
	virtual void Unbind();
	virtual void Draw();
	void BlitMultisampledBuffer() const;

private:
	GLuint frameBufferID;
	GLuint intermidiateFrameBufferID;
	GLuint colorTextureID;
	GLuint intermidiateTextureID;
	GLuint renderBufferID;
	std::unique_ptr<VertexArray> VAO;
};