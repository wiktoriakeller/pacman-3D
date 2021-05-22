#include "Renderer.h"

void Renderer::SetClearColor(const glm::vec4& color) const {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::ClearColor() const {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ClearDepth() const {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(GLsizei IBOcount) const {
	glDrawElements(GL_TRIANGLES, IBOcount, GL_UNSIGNED_INT, nullptr);
}