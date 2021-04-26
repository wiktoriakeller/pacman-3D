#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, GLsizeiptr size, GLenum usage, BufferLayout& bufferLayout) : layout(bufferLayout) {
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &bufferID);
}

VertexBuffer::VertexBuffer(VertexBuffer& other) : bufferID(other.bufferID), layout(other.layout) {
    other.bufferID = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    if (this != &other) {
        glDeleteBuffers(1, &bufferID);
        bufferID = other.bufferID;
        layout = other.layout;
        other.bufferID = 0;
    }

    return *this;
}

void VertexBuffer::SetBufferLayout(const BufferLayout& bufferLayout) {
    layout = bufferLayout;
}

void VertexBuffer::EnableLayout() const {
    layout.EnableElements();
}

void VertexBuffer::DisableLayout() const {
    layout.DisableElements();
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}