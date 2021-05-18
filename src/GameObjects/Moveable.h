#pragma once
#include "Entity.h"
#include "World.h"
#include <math.h>

class Moveable : public Entity
{
public:
	Moveable(std::unique_ptr<Model> model);
	bool SnapToGrid();
	void Move(float deltaTime);
	void CrossTunnel();

protected:
	const float SNAP_DISTANCE = 0.09f;
	glm::vec3 currentDirection;
	glm::vec3 startPosition;
	float speed;
	int nextX;
	int nextZ;
	bool shouldRotate;

private:
	int currentRotation;
};