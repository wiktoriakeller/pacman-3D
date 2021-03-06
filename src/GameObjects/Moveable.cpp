#include "Moveable.h"
    
Moveable::Moveable(std::unique_ptr<Model> model, bool createPointLight) : Entity(std::move(model), createPointLight) {
	currentRotation = 0;
	shouldRotate = false;
	speed = 0;
	nextX = 0;
	nextZ = 0;
	currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
}

bool Moveable::SnapToGrid() {
	glm::vec3 nextPosition = World::Instance().GetPosition(nextX, nextZ);
	glm::vec3 currentPosition = GetPosition();

	if (currentDirection.x != 0) {
		if (abs(nextPosition.x - currentPosition.x) < SNAP_DISTANCE) {
			SetPosition(nextPosition);
			return true;
		}
	}

	if (currentDirection.z != 0) {
		if (abs(nextPosition.z - currentPosition.z) < SNAP_DISTANCE) {
			SetPosition(nextPosition);
			return true;
		}
	}
	return false;
}

void Moveable::Move(float deltaTime) {
	SetPosition(GetPosition() + currentDirection * deltaTime * speed);
	
	if (shouldRotate) {
		Rotate(-currentRotation, glm::vec3(0.0f, 1.0f, 0.0f), true);
		if (currentDirection.x == 1) {
			currentRotation = 90;
		}
		else if (currentDirection.x == -1) {
			currentRotation = 270;
		}
		else if (currentDirection.z == 1) {
			currentRotation = 0;
		}
		else if (currentDirection.z == -1) {
			currentRotation = 180;
		}

		Rotate(currentRotation, glm::vec3(0.0f, 1.0f, 0.0f), true);
		shouldRotate = false;
	}
}

void Moveable::CrossTunnel() {
	if (nextX == World::Instance().TUNNEL_LEFT_POS.x) {
		SetPosition(World::Instance().GetPosition(World::Instance().TUNNEL_RIGHT_POS.x, World::Instance().TUNNEL_RIGHT_POS.y));
		nextX = World::Instance().TUNNEL_RIGHT_POS.x + 1;
	}
	else {
		SetPosition(World::Instance().GetPosition(World::Instance().TUNNEL_LEFT_POS.x, World::Instance().TUNNEL_LEFT_POS.y));
		nextX = World::Instance().TUNNEL_LEFT_POS.x - 1;
	}
}

glm::vec3 Moveable::GetCurrentDirection() const {
	return currentDirection;
}

glm::vec3 Moveable::GetCoordinates() const {
	return glm::vec3(nextX, 0.0f, nextZ);
}

bool Moveable::CanMakeMove(int x, int z) const {
	if (World::Instance().IsPositionValid(x, z) && World::Instance().GetMapElement(x, z) != MapElement::Wall) {
		return true;
	}
	return false;
}

bool Moveable::CanMakeMove(glm::vec3 coordinates) const {
	return CanMakeMove(coordinates.x, coordinates.z);
}

bool CheckCollision(std::shared_ptr<Moveable> object1, std::shared_ptr<Moveable> object2) {
	float distance = glm::distance(object1->GetPosition(), object2->GetPosition());
	if(distance < object1->COLLISION_DISTANCE) {
		return true;
	}

	return false;
}