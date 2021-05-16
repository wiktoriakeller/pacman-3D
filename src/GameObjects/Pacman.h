#pragma once
#include <functional>
#include "Entity.h"
#include "World.h"
#include "Points.h"
#include "../Input/KeyInput.h"

class Pacman : public Entity
{
public:
	Pacman(std::unique_ptr<Model> model, std::function<void(MapElement)> pointsAdder);
	void Update(float deltaTime) override;

private:
	const float SNAP_DISTANCE = 0.09f;
	bool stopped;
	float speed;
	int nextX;
	int nextZ;
	glm::vec3 wantedDirection;
	glm::vec3 currentDirection;
	std::function<void(MapElement)> addPoints;

	void EvaluatePoints() const;
	void EvaluateDirection();
	bool CanMakeMove(int x, int z);
	void ReverseDirection();
	void EvaluateMove();
	void SnapToGrid();
	void HandleInput();
	void Move(float deltaTime);
};