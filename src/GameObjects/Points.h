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
	const unsigned int CHERRY_SCORE = 100;
	const unsigned int GHOST_KILL_SCORE = 200;

	Points(std::unique_ptr<Model> model, std::unique_ptr<Entity> cherry, bool createPointLight);
	unsigned int GetScore();
	unsigned int GetLevel();
	unsigned int GetPointsLeft();
	bool GetIsCherrySpawned();
	bool GetIsCherryPicked();
	void Update(float deltaTime) override;
	void Draw(std::shared_ptr<Shader> shader) override;
	void Reset() override;
	void AddPoints(MapElement element, int x, int z);
	void ResetGhostScoreMultiplier();

private:
	const int START_POINTS = 246;
	const int POINTS_TO_SPAWN_CHERRY = 70;

	unsigned int level;
	unsigned int score;
	unsigned int ghostScoreMultiplier;
	int pointsLeft;
	int cherryX;
	int cherryZ;;
	float cherryTimer;
	float cherryDisappearTime;
	bool isCherrySpawned;
	bool isCherryPicked;
	std::unique_ptr<Entity> cherry;
};