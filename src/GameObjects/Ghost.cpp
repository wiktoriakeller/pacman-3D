#include "Ghost.h"

Ghost::Ghost(std::unique_ptr<Model> model, int startX, int startY, float houseTime, float houseOffset) : Moveable(std::move(model)), houseTime(houseTime) {
	nextX = startX;
	nextZ = startY;
	chaseTime = 20;
	scatterTime = 7;
	timer = 0;
	startPosition = World::Instance().GetPosition(nextX, nextZ);
	startPosition.x += houseOffset;
	SetPosition(startPosition);
	
	Scale(glm::vec3(0.7f, 0.7f, 0.7f));

	if (houseTime == 0) {
		currentState = State::Scatter;
	}
	else {
		currentState = State::House;
	}
}

void Ghost::Update(float deltaTime) {
	timer += deltaTime;
	ChangeState();
}

void Ghost::ChangeState() {
	switch (currentState)
	{
	case State::House:
		if (timer > houseTime) {
			currentState = State::Scatter;
			timer = 0;
		}
		break;
	case State::Scatter:
		if (timer > scatterTime) {
			currentState = State::Chase;
			timer = 0;
		}
		break;
	case State::Chase:
		if (timer > chaseTime) {
			currentState = State::Scatter;
			timer = 0;
		}
		break;
	default:
		break;
	}
}
