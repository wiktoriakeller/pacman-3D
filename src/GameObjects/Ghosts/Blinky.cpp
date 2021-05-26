#include "Blinky.h"

Blinky::Blinky(std::unique_ptr<Model> model, std::shared_ptr<Pacman> pacman, bool createPointLight) : 
	Ghost(std::move(model), pacman, createPointLight) {
	startX = 14;
	startZ = 14;
	cornerX = 26;
	cornerZ = 0;
	nextX = startX;
	nextZ = startZ;
	houseTime = 0.0f;
	startPosition = World::Instance().GetPosition(nextX, nextZ);
	currentDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	currentState = State::Scatter;
	startState = currentState;
	SetPosition(startPosition);
	TargetCorner();
}

void Blinky::PickTarget() {
	targetX = pacman->GetCoordinates().x;
	targetZ = pacman->GetCoordinates().z;
}