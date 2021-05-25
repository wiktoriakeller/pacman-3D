#pragma once
#include "Entity.h"
#include "World.h"

class Points : public Entity
{ 
public:
	const unsigned int POINT_SCORE = 10;
	const unsigned int POWER_SCORE = 50;

	Points(std::unique_ptr<Model> model);
	void Draw(std::shared_ptr<Shader> shader) override;
	void AddPoints(MapElement element);
	void Reset() override;
	static unsigned int GetScore();
	int GetPointsLeft();

private:
	int pointsLeft;
	static unsigned int score;
};