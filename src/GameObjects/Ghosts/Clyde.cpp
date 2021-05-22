#include "Clyde.h"

Clyde::Clyde(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman) : Ghost(std::move(model), pacman) {
	nextX = START_X;
	nextZ = START_Z;
	cornerX = 0;
	cornerZ = 35;
	startPosition = World::Instance().GetPosition(nextX, nextZ);
	SetPosition(startPosition);
	currentDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	currentState = State::House;
	targetX = nextX;
	targetZ = nextZ;
	houseTime = 20;
}

void Clyde::PickTarget() {
	if (glm::distance(GetPosition(), pacman->GetPosition()) > 8.0f ) {
		targetX = pacman->GetCoordinates().x;
		targetZ = pacman->GetCoordinates().z;
	}
	else {
		TargetCorner();
	}
}