#include "VertexArray.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &bufferID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &bufferID);
}

void VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) {
	Bind();
	VBO.emplace_back(std::move(vertexBuffer));
	VBO[VBO.size() - 1]->Bind();
	VBO[VBO.size() - 1]->EnableLayout();
}

void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) {
	Bind();
	IBO = indexBuffer;
	IBO->Bind();
}

unsigned int VertexArray::GetIBOCount() const {
	if (IBO != nullptr)
		return IBO->GetCount();

	return 0;
}

void VertexArray::Bind() const {
	glBindVertexArray(bufferID);
}

void VertexArray::Unbind() const {
	glBindVertexArray(bufferID);
}