#include "BufferLayout.h"

BufferLayoutElement::BufferLayoutElement(AttributeDataType type, GLint count, unsigned int offset, bool normalized) :
	Type(type), Count(count), Normalized(normalized), Offset(offset) { };

GLenum AttributeDataTypeToOpenGLType(AttributeDataType type) {
	switch (type) 
	{
		case AttributeDataType::Float:  return GL_FLOAT;
		case AttributeDataType::Float2: return GL_FLOAT;
		case AttributeDataType::Float3: return GL_FLOAT;
		case AttributeDataType::Float4: return GL_FLOAT;
		case AttributeDataType::Mat3f:  return GL_FLOAT;
		case AttributeDataType::Mat4f:  return GL_FLOAT;
		case AttributeDataType::Int:    return GL_INT;
		case AttributeDataType::Int2:   return GL_INT;
		case AttributeDataType::Int3:   return GL_INT;
		case AttributeDataType::Int4:   return GL_INT;
		case AttributeDataType::Bool:   return GL_BOOL;
	}
}

size_t AttributeDataTypeSize(AttributeDataType type) {
	switch (type) 
	{
		case AttributeDataType::Float:  return sizeof(GLfloat);
		case AttributeDataType::Float2: return sizeof(GLfloat);
		case AttributeDataType::Float3: return sizeof(GLfloat);
		case AttributeDataType::Float4: return sizeof(GLfloat);
		case AttributeDataType::Mat3f:  return 3 * sizeof(GLfloat);
		case AttributeDataType::Mat4f:  return 4 * sizeof(GLfloat);
		case AttributeDataType::Int:    return sizeof(GLint);
		case AttributeDataType::Int2:   return sizeof(GLint);
		case AttributeDataType::Int3:   return sizeof(GLint);
		case AttributeDataType::Int4:   return sizeof(GLint);
		case AttributeDataType::Bool:   return sizeof(GLboolean);
	}
}

BufferLayout::BufferLayout(const std::initializer_list<BufferLayoutElement>& elements) : elements(elements), stride(0) {
	CalculateStrideAndOffset();
}

void BufferLayout::EnableElements() const {
	for (int i = 0; i < elements.size(); i++) {
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, elements[i].Count, AttributeDataTypeToOpenGLType(elements[i].Type),
			elements[i].Normalized ? GL_TRUE : GL_FALSE, stride, (const void*) elements[i].Offset);
	}
}

void BufferLayout::DisableElements() const {
	for (int i = 0; i < elements.size(); i++) {
		glDisableVertexAttribArray(i);
	}
}

void BufferLayout::CalculateStrideAndOffset() {
	for (int i = 0; i < elements.size(); i++) {
		if(elements[i].Offset == 0)
			elements[i].Offset = stride;
		
		stride += elements[i].Count * AttributeDataTypeSize(elements[i].Type);
	}
}