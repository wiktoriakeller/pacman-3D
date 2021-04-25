#include "Renderer.h"

void Renderer::SetClearColor(const glm::vec4& color) const {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(std::shared_ptr<VertexArray> VAO) const {
	VAO->Bind();
	glDrawElements(GL_TRIANGLES, VAO->GetIBOCount(), GL_UNSIGNED_INT, nullptr);
}