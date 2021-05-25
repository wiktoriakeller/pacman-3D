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
	glm::vec3 GetCurrentDirection() const;
	glm::vec3 GetCoordinates() const;

protected:
	const float SNAP_DISTANCE = 0.12f;
	glm::vec3 currentDirection;
	glm::vec3 startPosition;
	float speed;
	int nextX;
	int nextZ;
	int START_X;
	int START_Z;
	bool shouldRotate;
	bool CanMakeMove(int x, int z) const;
	bool CanMakeMove(glm::vec3 coordinates) const;

private:
	int currentRotation;
};