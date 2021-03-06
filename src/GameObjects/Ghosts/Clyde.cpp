#include "Clyde.h"

Clyde::Clyde(std::unique_ptr<Model> model, std::shared_ptr<Pacman> pacman, bool createPointLight) : 
	Ghost(std::move(model), pacman, createPointLight) {
	startX = 12;
	startZ = 17;
	cornerX = 0;
	cornerZ = 35;
	nextX = startX;
	nextZ = startZ;
	targetX = nextX;
	targetZ = nextZ;
	houseTime = 15.0f;
	startPosition = World::Instance().GetPosition(nextX, nextZ);
	SetPosition(startPosition);
	currentDirection = glm::vec3(0.0f, 0.0f, 1.0f);
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