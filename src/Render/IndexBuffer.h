#pragma once
#include <GL\glew.h>

class IndexBuffer
{
public:
	IndexBuffer(const void* data, unsigned int count, GLenum usage);
	~IndexBuffer();

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	IndexBuffer(IndexBuffer& other);
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;

	unsigned int GetCount() const;
	void Bind() const;
	void Unbind() const;

private:
	GLuint bufferID;
	unsigned int count;
};