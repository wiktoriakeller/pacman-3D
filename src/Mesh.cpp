#include "Mesh.h"

Mesh::Mesh(Vertex* vertex, const unsigned int& verticesSize, GLuint* index, const unsigned int& indicesSize, unsigned int materialIndex) :
	materialIndex(materialIndex) {
	VAO = std::make_shared<VertexArray>();

	BufferLayout layout = {
		{AttributeDataType::Float3, 3},	//Position;
		{AttributeDataType::Float3, 3},	//Normal;
		{AttributeDataType::Float2, 2},	//TexCoords;
		{AttributeDataType::Float3, 3},	//Tangent;
		{AttributeDataType::Float3, 3}	//Bitangent;  
	};
		
	std::shared_ptr<VertexBuffer> VBO = std::make_shared<VertexBuffer>(vertex, verticesSize * sizeof(Vertex), GL_STATIC_DRAW, layout);
	VAO->AddVertexBuffer(std::move(VBO));

	std::shared_ptr<IndexBuffer> IBO = std::make_shared<IndexBuffer>(index, indicesSize, GL_STATIC_DRAW);
	VAO->SetIndexBuffer(std::move(IBO));
}

void Mesh::Draw(std::shared_ptr<Shader> shader) const {
	Renderer::Instance().Draw(VAO);
}

unsigned int Mesh::GetMaterialIndex() const {
	return materialIndex;
}