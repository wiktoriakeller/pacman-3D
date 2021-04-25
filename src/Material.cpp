#include "Material.h"

Material::Material(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess) :
	ambientColor(ambientColor), diffuseColor(diffuseColor), specularColor(specularColor), shininess(shininess) {
	diffuseTexture = nullptr;
	specularTexture = nullptr;
}

Material::Material(std::shared_ptr<Texture> diffuseTexture, std::shared_ptr<Texture> specularTexture, float shininess) :
	diffuseTexture(diffuseTexture), specularTexture(specularTexture), shininess(shininess) {
	ambientColor = glm::vec3(0);
	diffuseColor = glm::vec3(0);
	specularColor = glm::vec3(0);
}

void Material::SendMaterialToShader(std::shared_ptr<Shader> shader) const {
	shader->SetUniform("uMaterial.ambientColor", ambientColor);
	shader->SetUniform("uMaterial.diffuseColor", diffuseColor);
	shader->SetUniform("uMaterial.specularColor", specularColor);
	shader->SetUniform("uMaterial.shininess", shininess);
}

void Material::SendMaterialToShader(std::shared_ptr<Shader> shader, GLint diffuseSlot, GLint specularSlot) const {
	shader->SetUniform("uMaterial.diffuseTexture", diffuseSlot);
	shader->SetUniform("uMaterial.specularTexture", specularSlot);
	shader->SetUniform("uMaterial.shininess", shininess);

	diffuseTexture->Bind(diffuseSlot);
	specularTexture->Bind(specularSlot);
}