#include "Pinky.h"

Pinky::Pinky(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman) : Ghost(std::move(model), pacman) {
	startX = 14;
	startZ = 17;
	nextX = startX;
	nextZ = startZ;
	cornerX = 2;
	cornerZ = 0;
	startPosition = World::Instance().WorldCenter;
	SetPosition(startPosition);
	currentDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	currentState = State::House;
	startState = currentState;

	targetX = nextX;
	targetZ = nextZ;
	houseTime = 2;
}

void Pinky::PickTarget() {
	glm::vec3 futurePacmanCoords = pacman->GetCoordinates() + (pacman->GetCurrentDirection() * 4.0f);
	targetX = futurePacmanCoords.x;
	targetZ = futurePacmanCoords.z;
	targetX = std::clamp(targetX, 0, World::Instance().WIDTH - 1);
	targetZ = std::clamp(targetZ, 0, World::Instance().HEIGHT - 1);
}