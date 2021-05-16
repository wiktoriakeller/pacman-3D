#include "Camera.h"

Camera::Camera(std::shared_ptr<Entity> target) {
	targetObject = target;
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	positionOffset = glm::vec3(0.0f, 12.0f, 4.0f);
}

glm::mat4 Camera::GetView() {
	glm::vec3 targetPosition(std::clamp(targetObject->GetPosition().x, -10.0f, 10.0f), 0.5, std::clamp(targetObject->GetPosition().z, -16.0f, 13.0f));
	return glm::lookAt(targetPosition + positionOffset, targetPosition, up);
}

void Camera::SendToShader(std::shared_ptr<Shader> shader) {
	shader->SetUniform("uView", GetView());
	shader->SetUniform("uViewPosition", targetObject->GetPosition() + positionOffset);
}