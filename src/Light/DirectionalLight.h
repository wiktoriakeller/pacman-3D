#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);

	void SendToShader(std::shared_ptr<Shader> shader) override;
	void SetDirection(glm::vec3 newDirection);

private:
	glm::vec3 direction;
};