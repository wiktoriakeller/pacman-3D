#pragma once
#include <vector>
#include <string>
#include <GL\glew.h>

enum class AttributeDataType : unsigned char
{
	Float = 0, Float2, Float3, Float4, Mat3f, Mat4f, Int, Int2, Int3, Int4, Bool
};

GLenum AttributeDataTypeToOpenGLType(AttributeDataType type);
size_t AttributeDataTypeSize(AttributeDataType type);

struct BufferLayoutElement
{
	AttributeDataType Type;
	GLint Count;
	unsigned int Offset;
	bool Normalized;

	BufferLayoutElement(AttributeDataType type, GLint count, unsigned int offset = 0, bool normalized = false);
};

class BufferLayout
{
public:
	BufferLayout(const std::initializer_list<BufferLayoutElement>& elements);
	void EnableElements() const;
	void DisableElements() const;

private:
	GLsizei stride;
	std::vector<BufferLayoutElement> elements;

	void CalculateStrideAndOffset();
};