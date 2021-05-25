#include "Inky.h"

Inky::Inky(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman, std::shared_ptr<Moveable> blinky) : Ghost(std::move(model), pacman), blinky(blinky) {
	cornerX = 27;
	cornerZ = 35;
	START_X = 15;
	START_Z = 17;
	nextX = START_X;
	nextZ = START_Z;
	startPosition = World::Instance().GetPosition(nextX, nextZ);
	SetPosition(startPosition);
	currentState = State::House;
	startState = currentState;
	currentDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	targetX = nextX;
	targetZ = nextZ;
	houseTime = 15;
}

void Inky::PickTarget() {
	glm::vec3 pacmanFutureCoords = pacman->GetCoordinates() + pacman->GetCurrentDirection() * 2.0f;
	glm::vec3 target = blinky->GetCoordinates() + (pacmanFutureCoords - blinky->GetCoordinates())*2.0f;
	targetX = target.x;
	targetZ = target.z;
	targetX = std::clamp(targetX, 0, World::Instance().WIDTH - 1);
	targetZ = std::clamp(targetZ, 0, World::Instance().HEIGHT - 1);
}
