#include "Pacman.h"

#include <iostream>

Pacman::Pacman(std::shared_ptr<Model> model) : Entity(model) {
	speed = 4.0;
	nextX = 17;
	nextZ = 17;
	currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	wantedDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	stopped = true;
	SetPosition(World::Instance().GetPosition(nextX, nextZ));
}

void Pacman::Update(float deltaTime) {
	HandleInput();
	EvaluateDirection();
	Move(deltaTime);
}

void Pacman::HandleInput() {
	wantedDirection = glm::vec3(0.0f, 0.0f, 0.0f);

	switch (KeyInput::PressedKey)
	{
	case 'A':
		wantedDirection.x = 1;
		break;
	case 'D':
		wantedDirection.x = -1;
		break;
	case 'W':
		wantedDirection.z = 1;
		break;
	case 'S':
		wantedDirection.z = -1;
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
	else {
		SnapToGrid();
	}
}

void Pacman::ReverseDirection() {
	if (currentDirection.x > 0 && CanMakeMove(nextX - 1 + wantedDirection.x, nextZ + wantedDirection.z)) {
		currentDirection = wantedDirection;
		nextX = nextX - 1 + wantedDirection.x;
	}
	else if (currentDirection.x < 0 && CanMakeMove(nextX + 1 + wantedDirection.x, nextZ + wantedDirection.z)) {
		currentDirection = wantedDirection;
		nextX = nextX + 1 + wantedDirection.x;
	}
	else if (currentDirection.z > 0 && CanMakeMove(nextX + wantedDirection.x, nextZ - 1 + wantedDirection.z)) {
		currentDirection = wantedDirection;
		nextZ = nextZ - 1 + wantedDirection.z;
	}
	else if (currentDirection.z < 0 && CanMakeMove(nextX + wantedDirection.x, nextZ + 1 + wantedDirection.z)) {
		currentDirection = wantedDirection;
		nextZ = nextZ + 1 + wantedDirection.z;
	}
}

void Pacman::SnapToGrid() {
	float snapDistance =  0.1;
	glm::vec3 nextPosition = World::Instance().GetPosition(nextX, nextZ);
	glm::vec3 currentPosition = GetPosition();

	if(currentDirection.x != 0) {
		if(abs(nextPosition.x - currentPosition.x) < snapDistance) {
			SetPosition(nextPosition);
			EvaluateMove();
		}
	}
	
	if(currentDirection.z != 0) {
		if(abs(nextPosition.z - currentPosition.z) < snapDistance) {
			SetPosition(nextPosition);
			EvaluateMove();
		}
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
	}
	else if (CanMakeMove(nextX + currentDirection.x, nextZ + currentDirection.z)) {
		nextX += currentDirection.x;
		nextZ += currentDirection.z;
	}
	else {
		currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		stopped = true;
	}
}

void Pacman::Move(float deltaTime) {
	Translate(currentDirection * deltaTime * speed);
}