#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction) :
	Light(ambient, diffuse, specular), direction(direction) { }

void DirectionalLight::SendToShader(std::shared_ptr<Shader> shader) {
	shader->SetUniform("uDirLight.direction", direction);
	shader->SetUniform("uDirLight.ambient", ambient);
	shader->SetUniform("uDirLight.diffuse", diffuse);
	shader->SetUniform("uDirLight.specular", specular);
}

void DirectionalLight::SetDirection(glm::vec3 newDirection) {
	direction = newDirection;
}