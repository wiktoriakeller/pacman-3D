#pragma once
#include "Entity.h"
#include "World.h"
#include "../Input/KeyInput.h"

class Pacman : public Entity
{
public:
	Pacman(std::shared_ptr<Model> model);
	void Update(float deltaTime) override;

private:
	bool stopped;
	float speed;
	int nextX;
	int nextZ;
	glm::vec3 wantedDirection;
	glm::vec3 currentDirection;

	void EvaluateDirection();
	bool CanMakeMove(int x, int z);
	void ReverseDirection();
	void EvaluateMove();
	void SnapToGrid();
	void HandleInput();
	void Move(float deltaTime);
};