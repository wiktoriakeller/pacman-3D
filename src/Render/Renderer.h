#pragma once
#include <GL/glew.h>
#include <memory>
#include <GLM/vec4.hpp>
#include "VertexArray.h"

class Renderer
{
public:
	static Renderer& Instance() {
		static Renderer instance;
		return instance;
	}

	void SetClearColor(const glm::vec4& color) const;
	void Clear() const;
	void Draw(std::shared_ptr<VertexArray> VAO) const;

private:
	Renderer() {};
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	void operator=(const Renderer&) = delete;
	void operator=(const Renderer&&) = delete;
};