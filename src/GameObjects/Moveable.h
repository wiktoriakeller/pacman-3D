#pragma once
#include "Entity.h"
#include "World.h"

class Moveable : public Entity
{
public:
	const float SNAP_DISTANCE = 0.15f;

	Moveable(std::unique_ptr<Model> model);
	bool SnapToGrid();
	void Move(float deltaTime);
	void CrossTunnel();
	glm::vec3 GetCurrentDirection() const;
	glm::vec3 GetCoordinates() const;

protected:
	glm::vec3 currentDirection;
	glm::vec3 startPosition;
	float speed;
	int nextX;
	int nextZ;
	int startX;
	int startZ;
	int currentRotation;
	bool shouldRotate;

	bool CanMakeMove(int x, int z) const;
	bool CanMakeMove(glm::vec3 coordinates) const;
};

bool CheckCollision(std::shared_ptr<Moveable> object1, std::shared_ptr<Moveable> object2);