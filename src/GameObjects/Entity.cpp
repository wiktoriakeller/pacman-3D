#include "Entity.h"

Entity::Entity(std::unique_ptr<Model> objectModel) {
	model = std::move(objectModel);
	modelMatrix = glm::mat4(1.0f);
	UpdateNormalMatrix();
}

void Entity::Draw(std::shared_ptr<Shader> shader) {
	shader->SetUniform("uModel", modelMatrix);
	shader->SetUniform("uNormalMatrix", normalMatrix);
	model->Draw(shader);
}

glm::vec3 Entity::GetPosition() {
	return modelMatrix[3];
}

void Entity::SetPosition(glm::vec3 position) {
	modelMatrix[3].x = position.x;
	modelMatrix[3].y = position.y;
	modelMatrix[3].z = position.z;
}

void Entity::Translate(glm::vec3 translation) {
	modelMatrix = glm::translate(modelMatrix, translation);
	UpdateNormalMatrix();
}

void Entity::Rotate(float angle, glm::vec3 direction) {
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), direction);
	UpdateNormalMatrix();
}

void Entity::Scale(glm::vec3 factor) {
	modelMatrix = glm::scale(modelMatrix, factor);
	UpdateNormalMatrix();
}

void Entity::UpdateNormalMatrix() {
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
}

void Entity::Update(float deltaTime) { }