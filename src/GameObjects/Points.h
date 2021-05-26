#pragma once
#include <functional>

#include "Entity.h"
#include "World.h"
#include "../Game.h"

class Points : public Entity
{ 
public:
	const unsigned int POINT_SCORE = 10;
	const unsigned int POWER_SCORE = 50;
	const unsigned int GHOST_KILL_SCORE = 200;

	Points(std::unique_ptr<Model> model, bool createPointLight);
	unsigned int GetScore();
	unsigned int GetLevel();
	unsigned int GetPointsLeft();
	void Draw(std::shared_ptr<Shader> shader) override;
	void Reset() override;
	void AddPoints(MapElement element, int x, int z);
	void ResetGhostScoreMultiplier();

private:
	unsigned int level;
	unsigned int score;
	unsigned int pointsLeft;
	unsigned int ghostScoreMultiplier;
};