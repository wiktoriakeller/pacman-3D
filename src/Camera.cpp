#include "Camera.h"

Camera::Camera(std::shared_ptr<Entity> target) {
	targetObject = target;
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	positionOffset = glm::vec3(0.0f, 16.0f, 7.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)Game::WINDOW_WIDTH / Game::WINDOW_HEIGHT, 0.1f, 100.0f);
}

glm::mat4 Camera::GetView() {
	targetPosition = glm::vec3(std::clamp(targetObject->GetPosition().x, -7.0f, 7.0f), 0.5, std::clamp(targetObject->GetPosition().z, -11.0f, 11.0f));
	return glm::lookAt(targetPosition + positionOffset, targetPosition, up);
}

void Camera::SendToShader(std::shared_ptr<Shader> shader) {
	shader->SetUniform("uView", GetView());
	shader->SetUniform("uViewPosition", targetPosition + positionOffset);
	shader->SetUniform("uProjection", projection);
}