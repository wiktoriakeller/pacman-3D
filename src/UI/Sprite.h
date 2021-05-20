#pragma once
#include <GLM/glm.hpp>
#include <memory>

#include "../Render/Texture.h"
#include "../Render/Shader.h"
#include "../Render/Renderer.h"
#include "../Render/IndexBuffer.h"
#include "../Render/VertexArray.h"
#include "../Render/VertexBuffer.h"

class Sprite
{
public:
	Sprite(glm::vec3 color);
	Sprite(const std::string texturePath);
	void Draw(std::shared_ptr<Shader> shader);
	glm::vec2 GetPosition() const;
	glm::vec2 GetSize() const;
	void SetPosition(glm::vec2 position);
	void Translate(glm::vec2 translation);
	void SetScale(glm::vec2 factor);
	void Scale(glm::vec2 factor);

private:
	std::unique_ptr<VertexArray> VAO;
	std::unique_ptr<Texture> texture;
	glm::vec3 color;
	glm::mat4 modelMatrix;
};