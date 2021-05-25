#pragma once
#include "Entity.h"
#include "World.h"

class Points : public Entity
{ 
public:
	const unsigned int POINT_SCORE = 10;
	const unsigned int POWER_SCORE = 50;

	Points(std::unique_ptr<Model> model);
	static unsigned int GetScore();
	static unsigned int GetLevel();
	void Draw(std::shared_ptr<Shader> shader) override;
	void AddPoints(MapElement element, int x, int z);
	void Reset() override;
	unsigned int GetPointsLeft();

private:
	static unsigned int level;
	static unsigned int score;
	unsigned int pointsLeft;
};