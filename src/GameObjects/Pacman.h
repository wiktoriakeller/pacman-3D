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
	void DecreaseLives();
	void RestoreLives();
	int GetLives() const;
	bool GetPowerPillEffect() const;

private:
	int lives;
	bool stopped;
	bool powerPillEffect;
	glm::vec3 wantedDirection;
	std::function<void(MapElement, int, int)> addPoints;

	void EvaluatePoints();
	void EvaluateDirection();
	void ReverseDirection();
	void EvaluateMove();
	void HandleInput();
};