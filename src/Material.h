#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>
#include "Render/Shader.h"
#include "Render/Texture.h"

class Material
{
public:
	Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess);
	Material(std::shared_ptr<Texture> diffuseTexture, std::shared_ptr<Texture> specularTexture, float shininess);
	void SendMaterialToShader(std::shared_ptr<Shader> shader) const;
	void SendMaterialToShader(std::shared_ptr<Shader> shader, GLint diffuseSlot, GLint specularSlot) const;

private:
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;

	std::shared_ptr<Texture> diffuseTexture;
	std::shared_ptr<Texture> specularTexture;
};