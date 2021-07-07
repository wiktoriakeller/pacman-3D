#pragma once
#include <functional>

#include "Moveable.h"
#include "World.h"
#include "Points.h"
#include "../Input/KeyInput.h"

class Pacman : public Moveable
{
public:
	Pacman(std::unique_ptr<Model> model, std::function<void(MapElement, int, int)> pointsAdder, bool createPointLight);
	void Update(float deltaTime) override;
	void Reset() override;
	void Draw(std::shared_ptr<Shader> shader) override;
	void DecreaseLives();
	void RestoreLives();
	int GetLives() const;
	bool GetPowerPillEffect() const;

private:
	const float maxRotation = 28.0f;
	const float minRotation = 2.0f;

	int lives;
	bool stopped;
	bool powerPillEffect;
	glm::vec3 wantedDirection;
	std::function<void(MapElement, int, int)> addPoints;

	glm::mat4 topPart;
	glm::mat4 bottomPart;
	float animationRotationAngle;
	float animationRotationSpeed;

	void EvaluatePoints();
	void EvaluateDirection();
	void ReverseDirection();
	void EvaluateMove();
	void HandleInput();
	void Animate(float deltaTime);
};