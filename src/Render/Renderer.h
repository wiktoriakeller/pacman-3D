#pragma once
#include <GL/glew.h>
#include <memory>
#include <GLM/vec4.hpp>
#include "VertexArray.h"

enum class ClearType : unsigned char {
	Color, Depth, All
};

class Renderer
{
public:
	static Renderer& Instance() {
		static Renderer instance;
		return instance;
	}

	void SetClearColor(const glm::vec4& color) const;
	void Clear() const;
	void Clear(ClearType type) const;
	void Draw(GLsizei IBOcount) const;

private:
	Renderer() {};
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	void operator=(const Renderer&) = delete;
	void operator=(const Renderer&&) = delete;

	void ClearColor() const;
	void ClearDepth() const;
};