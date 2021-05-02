#include "Camera.h"

glm::vec3 Camera::GetPosition()
{
	return Target + PositionOffset;
}

Camera::Camera() {
	Target = glm::vec3(0.0f, 0.0f, 0.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	PositionOffset = glm::vec3(0.0f, 5.0f, -3.0f);
}


glm::mat4 Camera::GetView() {
	return glm::lookAt(Target + PositionOffset, Target, Up);
}


void Camera::LookAt(glm::mat4 model) {
	Target = model[3];
}