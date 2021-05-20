#include "Sprite.h"

Sprite::Sprite(glm::vec3 color) : color(color) {
	modelMatrix = glm::mat4(1.0f);

	//position, tex coord
	GLfloat vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 3, 1
	};

	BufferLayout layout = {
		{AttributeDataType::Float2, 2},	//Position
		{AttributeDataType::Float2, 2} //Tex coords
	};

	VAO = std::make_unique<VertexArray>();
	
	std::unique_ptr<VertexBuffer> VBO = std::make_unique<VertexBuffer>(vertices, sizeof(GLfloat) * 4 * 4, GL_STATIC_DRAW, layout);
	VAO->AddVertexBuffer(std::move(VBO));

	std::unique_ptr<IndexBuffer> IBO = std::make_unique<IndexBuffer>(indices, 6, GL_STATIC_DRAW);
	VAO->SetIndexBuffer(std::move(IBO));
}

Sprite::Sprite(const std::string texturePath) : Sprite(glm::vec3(1.0f, 0.0f, 1.0f)) {
	texture = std::make_unique<Texture>(texturePath);
}

void Sprite::Draw(std::shared_ptr<Shader> shader) {
	shader->SetUniform("uModel", modelMatrix);

	if (texture != nullptr) {
		shader->SetUniform("useColor", false);
		texture->Bind(0);
		VAO->Bind();
		Renderer::Instance().Draw(VAO->GetIBOCount());
		texture->Unbind();
	}
	else {
		shader->SetUniform("useColor", true);
		shader->SetUniform("uColor", color);
		VAO->Bind();
		Renderer::Instance().Draw(VAO->GetIBOCount());
	}
}

glm::vec2 Sprite::GetPosition() const {
	return glm::vec2(modelMatrix[3].x, modelMatrix[3].y);
}

glm::vec2 Sprite::GetSize() const {
	return glm::vec2(modelMatrix[0].x, modelMatrix[1].y);
}

void Sprite::SetPosition(glm::vec2 position) {
	modelMatrix[3].x = position.x;
	modelMatrix[3].y = position.y;
}

void Sprite::Translate(glm::vec2 translation) {
	modelMatrix = glm::translate(modelMatrix, glm::vec3(translation, 0.0f));
}

void Sprite::SetScale(glm::vec2 factor) {
	modelMatrix[0].x = factor.x;
	modelMatrix[1].y = factor.y;
}

void Sprite::Scale(glm::vec2 factor) {
	modelMatrix = glm::scale(modelMatrix, glm::vec3(factor, 1.0f));
}