#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position,
		float constant, float linear, float quadratic);
	~PointLight();
	void SendToShader(std::shared_ptr<Shader> shader) override;
	void SetPosition(glm::vec3 newPosition);
	glm::vec3 GetPosition() const;
	unsigned int GetNumberOfPointLights() const;

private:
	static unsigned int numberOfPointLights;

	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	unsigned int index;
};