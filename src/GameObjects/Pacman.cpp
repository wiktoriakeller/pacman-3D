#include "Pacman.h"

Pacman::Pacman(std::unique_ptr<Model> model, std::function<void(MapElement)> pointsAdder) : Entity(std::move(model)) {
	addPoints = pointsAdder;
	speed = 5.0;
	nextX = 17;
	nextZ = 17;
	currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	wantedDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	stopped = true;
	Scale(glm::vec3(0.8f, 0.8f, 0.8f));
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
	glm::vec3 nextPosition = World::Instance().GetPosition(nextX, nextZ);
	glm::vec3 currentPosition = GetPosition();

	if(currentDirection.x != 0) {
		if(abs(nextPosition.x - currentPosition.x) < SNAP_DISTANCE) {
			SetPosition(nextPosition);
			EvaluatePoints();
			EvaluateMove();
		}
	}
	
	if(currentDirection.z != 0) {
		if(abs(nextPosition.z - currentPosition.z) < SNAP_DISTANCE) {
			SetPosition(nextPosition);
			EvaluatePoints();
			EvaluateMove();
		}
	}
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
	}
	else if (CanMakeMove(nextX + currentDirection.x, nextZ + currentDirection.z)) {
		nextX += currentDirection.x;
		nextZ += currentDirection.z;
	}
	else if (!CanMakeMove(nextX + currentDirection.x, nextZ + currentDirection.z) && World::Instance().GetMapElement(nextX, nextZ) == MapElement::Tunnel) {
		if (nextX == World::Instance().TUNNEL_LEFT_POS.x) {
			SetPosition(World::Instance().GetPosition(World::Instance().TUNNEL_RIGHT_POS.x, World::Instance().TUNNEL_RIGHT_POS.y));
			nextX = World::Instance().TUNNEL_RIGHT_POS.x + 1;
		}
		else {
			SetPosition(World::Instance().GetPosition(World::Instance().TUNNEL_LEFT_POS.x, World::Instance().TUNNEL_LEFT_POS.y));
			nextX = World::Instance().TUNNEL_LEFT_POS.x - 1;
		}
	}
	else {
		currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		stopped = true;
	}
}

void Pacman::Move(float deltaTime) {
	Translate(currentDirection * deltaTime * speed);
}