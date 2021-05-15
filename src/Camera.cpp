#include "Camera.h"

Camera::Camera(std::shared_ptr<Entity> target) {
	targetObject = target;
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	positionOffset = glm::vec3(0.0f, 12.0f, -4.0f);
}

glm::mat4 Camera::GetView() {
	return glm::lookAt(targetObject->GetPosition() + positionOffset, targetObject->GetPosition(), up);
}

void Camera::SendToShader(std::shared_ptr<Shader> shader) {
	shader->SetUniform("uView", GetView());
	shader->SetUniform("uViewPosition", targetObject->GetPosition() + positionOffset);
}