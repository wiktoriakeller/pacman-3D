#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count, GLenum usage) : count(count) {
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, usage);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &bufferID);
}

IndexBuffer::IndexBuffer(IndexBuffer& other) : bufferID(other.bufferID), count(other.count) {
	other.bufferID = 0;
	other.count = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
	if (this != &other) {
		glDeleteBuffers(1, &bufferID);

		bufferID = other.bufferID;
		count = other.count;

		other.bufferID = 0;
		other.count = 0;
	}

	return *this;
}

unsigned int IndexBuffer::GetCount() const {
	return count;
}

void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

void IndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}