#pragma once
#include "Entity.h"
#include "World.h"
#include "../Game.h"

class Points : public Entity
{ 
public:
	const unsigned int POINT_SCORE = 10;
	const unsigned int POWER_SCORE = 50;

	Points(std::unique_ptr<Model> model);
	unsigned int GetScore();
	unsigned int GetLevel();
	unsigned int GetPointsLeft();
	void Draw(std::shared_ptr<Shader> shader) override;
	void AddPoints(MapElement element, int x, int z);
	void Reset() override;

private:
	unsigned int level;
	unsigned int score;
	unsigned int pointsLeft;
};