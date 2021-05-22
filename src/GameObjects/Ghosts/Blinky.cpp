#include "Blinky.h"

Blinky::Blinky(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman) : Ghost(std::move(model), pacman) {
	nextX = START_X;
	nextZ = START_Z;
	cornerX = 26;
	cornerZ = 0;
	currentState = State::Scatter;
	startPosition = World::Instance().GetPosition(nextX, nextZ);
	SetPosition(startPosition);
	TargetCorner();
	currentDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	houseTime = 0;
}

void Blinky::PickTarget() {
	targetX = pacman->GetCoordinates().x;
	targetZ = pacman->GetCoordinates().z;
}
