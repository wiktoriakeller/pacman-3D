#pragma once
#include <GL/glew.h>
#include <algorithm>

#include "GameObjects/Entity.h"
#include "Game.h"

class Camera
{
public:
	Camera(std::shared_ptr<Entity> target);
	void SendToShader(std::shared_ptr<Shader> shader);
	glm::mat4 GetView();

private:
	std::shared_ptr<Entity> targetObject;
	glm::vec3 up;
	glm::vec3 positionOffset;
	glm::mat4 projection;
	glm::vec3 targetPosition;
};