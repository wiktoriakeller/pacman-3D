#include "Mesh.h"

Mesh::Mesh(Vertex* vertex, unsigned int verticesSize, GLuint* index, unsigned int indicesSize, unsigned int matIndex) :
	materialIndex(matIndex) {
	
	VAO = std::make_unique<VertexArray>();

	BufferLayout layout = {
		{AttributeDataType::Float3, 3},	//Position;
		{AttributeDataType::Float3, 3},	//Normal;
		{AttributeDataType::Float2, 2},	//TexCoords;
		{AttributeDataType::Float3, 3},	//Tangent;
		{AttributeDataType::Float3, 3}	//Bitangent;
	};

	std::unique_ptr<VertexBuffer> VBO = std::make_unique<VertexBuffer>(vertex, verticesSize * sizeof(Vertex), GL_STATIC_DRAW, layout);
	VAO->AddVertexBuffer(std::move(VBO));

	std::unique_ptr<IndexBuffer> IBO = std::make_unique<IndexBuffer>(index, indicesSize, GL_STATIC_DRAW);
	VAO->SetIndexBuffer(std::move(IBO));
}

void Mesh::Draw() const {
	VAO->Bind();
	Renderer::Instance().Draw(VAO->GetIBOCount());
}

unsigned int Mesh::GetMaterialIndex() const {
	return materialIndex;
}

void Mesh::SetMaterialIndex(unsigned int newMaterialIndex) {
	materialIndex = newMaterialIndex;
}