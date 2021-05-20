#include "Pacman.h"

unsigned int Pacman::lives = 2;

Pacman::Pacman(std::unique_ptr<Model> model, std::function<void(MapElement)> pointsAdder) : Moveable(std::move(model)) {
	addPoints = pointsAdder;
	speed = 6.0;
	nextX = 14;
	nextZ = 23;
	currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	wantedDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	stopped = true;
	Rotate(-30, glm::vec3(1.0f, 0.0f, 0.0f));
	Scale(glm::vec3(0.8f, 0.8f, 0.8f));
	SetPosition(World::Instance().GetPosition(nextX, nextZ));
}

void Pacman::Update(float deltaTime) {
	HandleInput();
	EvaluateDirection();
	Move(deltaTime);
}

unsigned int Pacman::GetLives() {
	return lives;
}

void Pacman::HandleInput() {
	wantedDirection = glm::vec3(0.0f, 0.0f, 0.0f);

	switch (KeyInput::PressedKey)
	{
	case 'A':
		wantedDirection.x = -1;
		break;
	case 'D':
		wantedDirection.x = 1;
		break;
	case 'W':
		wantedDirection.z = -1;
		break;
	case 'S':
		wantedDirection.z = 1;
		break;
	}

	if(stopped && CanMakeMove(nextX + wantedDirection.x, nextZ + wantedDirection.z) && wantedDirection != glm::vec3(0.0f, 0.0f, 0.0f)) {
		EvaluateMove();
		stopped = false;
	}
}

void Pacman::EvaluateDirection() {
	if (currentDirection == -wantedDirection && currentDirection != glm::vec3(0.0f, 0.0f, 0.0f)) {
		ReverseDirection();
	}
	else if(SnapToGrid()) {
		EvaluatePoints();
		EvaluateMove();
	}
}

void Pacman::ReverseDirection() {
	if (currentDirection.x > 0 && CanMakeMove(nextX - 1 + wantedDirection.x, nextZ + wantedDirection.z)) {
		nextX = nextX - 1 + wantedDirection.x;
	}
	else if (currentDirection.x < 0 && CanMakeMove(nextX + 1 + wantedDirection.x, nextZ + wantedDirection.z)) {
		nextX = nextX + 1 + wantedDirection.x;
	}
	else if (currentDirection.z > 0 && CanMakeMove(nextX + wantedDirection.x, nextZ - 1 + wantedDirection.z)) {
		nextZ = nextZ - 1 + wantedDirection.z;
	}
	else if (currentDirection.z < 0 && CanMakeMove(nextX + wantedDirection.x, nextZ + 1 + wantedDirection.z)) {
		nextZ = nextZ + 1 + wantedDirection.z;
	}

	currentDirection = wantedDirection;
	shouldRotate = true;
}

void Pacman::EvaluatePoints() const {
	MapElement element = World::Instance().GetMapElement(nextX, nextZ);

	if (element == MapElement::Point || element == MapElement::Power) {
		addPoints(element);
		World::Instance().SetMapElement(nextX, nextZ, MapElement::None);
	}
}

bool Pacman::CanMakeMove(int x, int z) {
	if (World::Instance().IsPositionValid(x, z) && World::Instance().GetMapElement(x, z) != MapElement::Wall) {
		return true;
	}

	return false;
}

void Pacman::EvaluateMove() {
	if (CanMakeMove(nextX + wantedDirection.x, nextZ + wantedDirection.z)) {
		currentDirection = wantedDirection;
		nextX += currentDirection.x;
		nextZ += currentDirection.z;
		shouldRotate = true;
	}
	else if (CanMakeMove(nextX + currentDirection.x, nextZ + currentDirection.z)) {
		nextX += currentDirection.x;
		nextZ += currentDirection.z;
	}
	else if (World::Instance().GetMapElement(nextX, nextZ) == MapElement::Tunnel) {
		CrossTunnel();
	}
	else {
		currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		stopped = true;
	}
}