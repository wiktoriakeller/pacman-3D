#include "Entity.h"

Entity::Entity(std::unique_ptr<Model> model, bool createPointLight) : model(std::move(model)) {
	modelMatrix = glm::mat4(1.0f);
	UpdateNormalMatrix();
	if (createPointLight) {
		pointLight = std::make_unique<PointLight>(glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.22f, 0.20f);
	}
	else {
		pointLight = nullptr;
	}
}

void Entity::Draw(std::shared_ptr<Shader> shader) {
	shader->SetUniform("uModel", modelMatrix);
	shader->SetUniform("uNormalMatrix", normalMatrix);
	model->Draw(shader);
}

glm::vec3 Entity::GetPosition() const {
	return modelMatrix[3];
}

void Entity::SetPosition(glm::vec3 position) {
	modelMatrix[3].x = position.x;
	modelMatrix[3].y = position.y;
	modelMatrix[3].z = position.z;
	UpdateNormalMatrix();
}

void Entity::Translate(glm::vec3 translation) {
	modelMatrix = glm::translate(modelMatrix, translation);
	UpdateNormalMatrix();
}

void Entity::Rotate(float angle, glm::vec3 direction, bool castToRadians) {
	if(castToRadians) {
		angle = glm::radians(angle);
	}

	modelMatrix = glm::rotate(modelMatrix, angle, direction);
	UpdateNormalMatrix();
}

void Entity::SetScale(glm::vec3 factor) {
	modelMatrix[0].x = factor.x;
	modelMatrix[1].y = factor.y;
	modelMatrix[2].z = factor.z;
	UpdateNormalMatrix();
}

void Entity::Scale(glm::vec3 factor) {
	modelMatrix = glm::scale(modelMatrix, factor);
	UpdateNormalMatrix();
}

void Entity::SendLightToShader(std::shared_ptr<Shader> shader) {
	if (pointLight != nullptr) {
		pointLight->SetPosition(GetPosition().x, GetPosition().y - 0.5f, GetPosition().z);
		pointLight->SendToShader(shader);
	}
}

void Entity::UpdateNormalMatrix() {
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
}

void Entity::Update(float deltaTime) { }

void Entity::Reset() {}