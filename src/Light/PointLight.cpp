#include "PointLight.h"

unsigned int PointLight::numberOfPointLights = 0;

PointLight::PointLight(glm::vec3 color, float intensity, glm::vec3 ambient, 
	glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, 
	float constant, float linear, float quadratic) :
	Light(color, intensity, ambient, diffuse, specular), 
	position(position),
	constant(constant),
	linear(linear),
	quadratic(quadratic) { 
		index = numberOfPointLights++;
	}

PointLight::~PointLight() { 
	numberOfPointLights--;
}

void PointLight::SendToShader(std::shared_ptr<Shader> shader) {
	const std::string name = "uPointLights[" + std::to_string(index) + "].";

	shader->SetUniform(name + "position", position);
	shader->SetUniform(name + "ambient", ambient);
	shader->SetUniform(name + "diffuse", diffuse);
	shader->SetUniform(name + "specular", specular);
	shader->SetUniform(name + "constant", constant);
	shader->SetUniform(name + "linear", linear);
	shader->SetUniform(name + "quadratic", quadratic);
}

void PointLight::SetPosition(glm::vec3 newPosition) {
	position = newPosition;
}

glm::vec3 PointLight::GetPosition() const {
	return position;
}

unsigned int PointLight::GetNumberOfPointLights() const {
	return numberOfPointLights;
}