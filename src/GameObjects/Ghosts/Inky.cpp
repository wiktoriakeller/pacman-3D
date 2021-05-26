#include "Inky.h"

Inky::Inky(std::unique_ptr<Model> model, std::shared_ptr<Pacman> pacman, std::shared_ptr<Moveable> blinky) : Ghost(std::move(model), pacman), blinky(blinky) {
	startX = 15;
	startZ = 17;
	cornerX = 27;
	cornerZ = 35;
	nextX = startX;
	nextZ = startZ;
	targetX = nextX;
	targetZ = nextZ;
	houseTime = 10.0f;
	startPosition = World::Instance().GetPosition(nextX, nextZ);
	SetPosition(startPosition);
	currentDirection = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Inky::PickTarget() {
	glm::vec3 pacmanFutureCoords = pacman->GetCoordinates() + pacman->GetCurrentDirection() * 2.0f;
	glm::vec3 target = blinky->GetCoordinates() + (pacmanFutureCoords - blinky->GetCoordinates())*2.0f;
	targetX = target.x;
	targetZ = target.z;
	targetX = std::clamp(targetX, 0, World::Instance().WIDTH - 1);
	targetZ = std::clamp(targetZ, 0, World::Instance().HEIGHT - 1);
}