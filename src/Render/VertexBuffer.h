#pragma once
#include <GL\glew.h>
#include "BufferLayout.h"

class VertexBuffer
{
public:
	VertexBuffer(const void* data, GLsizeiptr size, GLenum usage, const BufferLayout& bufferLayout);
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	VertexBuffer(VertexBuffer& other);
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void SetBufferLayout(const BufferLayout& bufferLayout);
	void ChangeData(unsigned int size, const void* data);
	void EnableLayout() const;
	void DisableLayout() const;
	void Bind() const;
	void Unbind() const;

private:
	GLuint bufferID;
	BufferLayout layout;
};