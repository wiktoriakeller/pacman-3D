#pragma once
#include <functional>
#include "Moveable.h"
#include "World.h"
#include "Points.h"
#include "../Input/KeyInput.h"

class Pacman : public Moveable
{
public:
	Pacman(std::unique_ptr<Model> model, std::function<void(MapElement, int, int)> pointsAdder);
	void Update(float deltaTime) override;
	void Reset() override;
	int GetLives();
	void DecreaseLives();
	void RestoreLives();

private:
	int lives;
	bool stopped;
	glm::vec3 wantedDirection;
	std::function<void(MapElement, int, int)> addPoints;

	void EvaluatePoints() const;
	void EvaluateDirection();
	void ReverseDirection();
	void EvaluateMove();
	void HandleInput();
};