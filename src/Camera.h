#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

class Camera
{
	glm::vec3 Position;
	glm::vec3 Target;
	glm::vec3 Up;
	glm::vec3 PositionOffset;

public:
	glm::vec3 GetPosition();
	Camera();
	glm::mat4 GetView();
	void LookAt(glm::mat4 model);
};