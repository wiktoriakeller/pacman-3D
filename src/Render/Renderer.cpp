#include "Renderer.h"

void Renderer::SetClearColor(const glm::vec4& color) const {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Clear(ClearType type) const {
	switch (type)
	{
	case ClearType::Color:
		ClearColor();
		break;

	case ClearType::Depth:
		ClearDepth();
		break;

	case ClearType::All:
		Clear();
		break;

	default:
		break;
	}
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