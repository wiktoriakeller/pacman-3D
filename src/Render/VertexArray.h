#pragma once
#include <memory>
#include <vector>
#include <GL\glew.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
	void SetIndexBuffer(std::shared_ptr<IndexBuffer> elementBuffer);
	unsigned int GetIBOCount() const;
	void Bind() const;
	void Unbind() const;

private:
	GLuint bufferID;

	std::vector<std::shared_ptr<VertexBuffer>> VBO;
	std::shared_ptr<IndexBuffer> IBO;
};