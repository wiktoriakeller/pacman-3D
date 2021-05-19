#include "VertexArray.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &bufferID);
	IBO.reset();
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &bufferID);
}

void VertexArray::AddVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer) {
	Bind();
	VBO.emplace_back(std::move(vertexBuffer));
	VBO[VBO.size() - 1]->Bind();
	VBO[VBO.size() - 1]->EnableLayout();
}

void VertexArray::ChangeVBOData(unsigned int VBOindex, unsigned int size, const void* data) {
	VBO[VBOindex]->ChangeData(size, data);
}

void VertexArray::BindVBO(unsigned int VBOindex) {
	VBO[VBOindex]->Bind();
}

void VertexArray::SetIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) {
	Bind();
	IBO = std::move(indexBuffer);
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