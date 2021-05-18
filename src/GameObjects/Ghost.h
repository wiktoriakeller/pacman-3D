#pragma once
#include "Moveable.h"

enum class State : unsigned char
{
	House = 0, Scatter, Chase
};

class Ghost : public Moveable
{
public:
	Ghost(std::unique_ptr<Model> model, int startX, int startY, float houseTime, float houseOffset);
	void Update(float deltaTime) override;
	void ChangeState();

private:
	State currentState;
	float houseTime;
	float chaseTime;
	float scatterTime;
	float timer;
};