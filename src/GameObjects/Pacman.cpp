#include "Pacman.h"

Pacman::Pacman(std::unique_ptr<Model> model, std::function<void(MapElement, int, int)> pointsAdder,
	bool createPointLight) : Moveable(std::move(model), createPointLight) {
	addPoints = pointsAdder;
	speed = 7.0f;
	startX = 14;
	startZ = 26;
	lives = 2;
	nextX = startX;
	nextZ = startZ;
	startPosition = World::Instance().GetPosition(nextX, nextZ);
	currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	wantedDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	stopped = true;
	powerPillEffect = false;
	Rotate(-30, glm::vec3(1.0f, 0.0f, 0.0f));
	Scale(glm::vec3(0.8f, 0.8f, 0.8f));
	SetPosition(startPosition);
}

void Pacman::Update(float deltaTime) {
	powerPillEffect = false;
	HandleInput();
	EvaluateDirection();
	Move(deltaTime);
}

void Pacman::Reset() {
	SetPosition(startPosition);
	currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	wantedDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	nextX = startX;
	nextZ = startZ;
	stopped = true;
	shouldRotate = true;
}

int Pacman::GetLives() const {
	return lives;
}

void Pacman::DecreaseLives() {
	if (lives >= 0) {
		lives--;
	}
}

void Pacman::RestoreLives() {
	lives = 2;
}

bool Pacman::GetPowerPillEffect() const {
	return powerPillEffect;
}

void Pacman::HandleInput() {
	wantedDirection = glm::vec3(0.0f, 0.0f, 0.0f);

	switch (KeyInput::GetPressedKey())
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
	if(SnapToGrid()) {
		EvaluatePoints();
		EvaluateMove();
	}
	else if (currentDirection == -wantedDirection && currentDirection != glm::vec3(0.0f, 0.0f, 0.0f) && 
		CanMakeMove(nextX + wantedDirection.x, nextZ + wantedDirection.z)) {
		ReverseDirection();
	}
}

void Pacman::ReverseDirection() {
	if (currentDirection.x != 0 ) {
		nextX = nextX + wantedDirection.x;
	}
	else if (currentDirection.z != 0) {
		nextZ = nextZ + wantedDirection.z;
	}

	currentDirection = wantedDirection;
	shouldRotate = true;
}

void Pacman::EvaluatePoints() {
	MapElement element = World::Instance().GetMapElement(nextX, nextZ);

	if (element == MapElement::Point || element == MapElement::Power) {
		addPoints(element, nextX, nextZ);

		if (element == MapElement::Power) {
			powerPillEffect = true;
		}
	}
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