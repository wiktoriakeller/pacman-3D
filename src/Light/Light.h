#pragma once
#include <memory>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "../Render/Shader.h"

class Light
{
public:
	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	virtual void SendToShader(std::shared_ptr<Shader> shader) = 0;

	glm::vec3 GetAmbient() const;
	glm::vec3 GetDiffuse() const;
	glm::vec3 GetSpecular() const;

protected:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};