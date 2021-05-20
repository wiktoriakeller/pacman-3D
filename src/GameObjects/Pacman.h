#pragma once
#include <functional>
#include "Moveable.h"
#include "World.h"
#include "Points.h"
#include "../Input/KeyInput.h"

class Pacman : public Moveable
{
public:
	Pacman(std::unique_ptr<Model> model, std::function<void(MapElement)> pointsAdder);
	void Update(float deltaTime) override;
	static unsigned int GetLives();

private:
	static unsigned int lives;
	bool stopped;
	glm::vec3 wantedDirection;
	std::function<void(MapElement)> addPoints;

	void EvaluatePoints() const;
	void EvaluateDirection();
	bool CanMakeMove(int x, int z);
	void ReverseDirection();
	void EvaluateMove();
	void HandleInput();
};