#pragma once
#include <memory>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "../Render/Shader.h"

class Light
{
public:
	Light(glm::vec3 color, float intensity, glm::vec3 ambient,
		glm::vec3 diffuse, glm::vec3 specular) :
		color(color),
		intensity(intensity),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular) {};
	~Light() {};

	virtual void SendToShader(std::shared_ptr<Shader> shader) = 0;

protected:
	glm::vec3 color;
	float intensity;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};