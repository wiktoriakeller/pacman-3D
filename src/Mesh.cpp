#include "Mesh.h"

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoords, glm::vec3 tangent, glm::vec3 bitangent) :
	Position(position),
	Normal(normal),
	TextureCoords(textureCoords),
	Tangent(tangent),
	Bitangent(bitangent) { }

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, unsigned int materialIndex) :
	vertices(vertices),
	indices(indices),
	materialIndex(materialIndex) {
	VAO = std::make_shared<VertexArray>();

	BufferLayout layout = {
		{AttributeDataType::Float3, 3},	//Position;
		{AttributeDataType::Float3, 3},	//Normal;
		{AttributeDataType::Float2, 2},	//TexCoords;
		{AttributeDataType::Float3, 3},	//Tangent;
		{AttributeDataType::Float3, 3}	//Bitangent;  
	};
		
	std::shared_ptr<VertexBuffer> VBO = std::make_shared<VertexBuffer>(&vertices[0], vertices.size() * sizeof(Vertex), GL_STATIC_DRAW, layout);
	VAO->AddVertexBuffer(std::move(VBO));

	std::shared_ptr<IndexBuffer> IBO = std::make_shared<IndexBuffer>(&indices[0], indices.size(), GL_STATIC_DRAW);
	VAO->SetIndexBuffer(std::move(IBO));
}

void Mesh::Draw(std::shared_ptr<Shader> shader) const {
	Renderer::Instance().Draw(VAO);
}

unsigned int Mesh::GetMaterialIndex() const {
	return materialIndex;
}