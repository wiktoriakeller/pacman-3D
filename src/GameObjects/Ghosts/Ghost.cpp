#include "Ghost.h"

Ghost::Ghost(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman) : Moveable(std::move(model)), pacman(pacman) {
	chaseTime = 50;
	scatterTime = 10;
	speed = 4.0f;
	baseSpeed = speed;
	timer = 0;
	Scale(glm::vec3(0.7f, 0.7f, 0.7f));
	Rotate(90, glm::vec3(0.0f, 1.0f, 0.0f));
	leavingState = LeavingState::CenterX;
}

void Ghost::Update(float deltaTime) {
	timer += deltaTime;
	UpdateState();
	HandleMovement(deltaTime);
}

void Ghost::HandleMovement(float deltaTime) {
	
	switch (currentState) {
	case State::House:
		break;

	case State::Leaving:
		LeaveHouse();
		break;

	default:
		if (SnapToGrid()) {
			PickPath(false);
		}
		break;

	}
	if (World::Instance().GetMapElement(nextX, nextZ) == MapElement::Tunnel) {
		CrossTunnel();
	}
	Move(deltaTime);
}

void Ghost::UpdateState() {
	switch (currentState) {
	case State::House:
		speed = 0.0f;
		if (timer > houseTime) {
			speed = baseSpeed;
			ChangeState(State::Leaving);
		}
		break;

	case State::Scatter:
		TargetCorner();
		if (timer > scatterTime) {
			ChangeState(State::Chase);
		}
		break;

	case State::Chase:
		PickTarget();
		if (timer > chaseTime) {
			ChangeState(State::Scatter);
		}
		break;

	case State::Frightened:
		TargetRandom();
		if (timer > frightenedTime) {
			ChangeState(State::Chase);
		}
	default:
		break;
	}
}

void Ghost::ChangeState(State newState) {
	currentState = newState;
	timer = 0;
}

void Ghost::LeaveHouse() {
	shouldRotate = true;

	switch (leavingState)
	{
	case LeavingState::CenterX:
		if (!World::Instance().ArePositionsEqual(GetPosition(), World::Instance().WorldCenter)) {
			float direction = 1.0f;
			if (GetPosition().x > World::Instance().WorldCenter.x) {
				direction = -1.0f;
			}
			currentDirection = glm::vec3(direction, 0.0f, 0.0f);
		}
		else {
			leavingState = LeavingState::CenterZ;
		}
		break;

	case LeavingState::CenterZ:
		if (!World::Instance().ArePositionsEqual(GetPosition(), glm::vec3(GetPosition().x, 0.0f, World::Instance().GetPosition(0, HOUSE_CENTER_Z - 3).z))) {
			currentDirection = glm::vec3(0.0f, 0.0f, -1.0f);
		}
		else {
			leavingState = LeavingState::BackToNormal;
		}
		break;

	case LeavingState::BackToNormal:
		nextZ = HOUSE_CENTER_Z - 3;
		nextX = HOUSE_CENTER_X;
		ChangeState(State::Scatter);
		UpdateState();
		PickPath(false);
		break;

	default:
		break;
	}
}

void Ghost::PickPath(bool canTurnAround) {
	float minDistance = std::numeric_limits<float>::max();
	int bestDirectionIndex = 0;
	for (int i = 0; i < directions.size(); i++) {
		if (!canTurnAround) {
			if (currentDirection.x != 0) {
				if (directions[i].x == currentDirection.x * -1) {
					continue;
				}
			}
			else if (currentDirection.z != 0) {
				if (directions[i].z == currentDirection.z * -1) {
					continue;
				}
			}
		}
		if (CanMakeMove(GetCoordinates() + directions[i])) {
			glm::vec3 futurePosition = World::Instance().GetPosition(GetCoordinates().x + directions[i].x, GetCoordinates().z + directions[i].z);
			float distance = glm::distance(futurePosition, World::Instance().GetPosition(targetX, targetZ));
			if (distance < minDistance) {
				minDistance = distance;
				bestDirectionIndex = i;
			}
		}
	}
	currentDirection = directions[bestDirectionIndex];
	shouldRotate = true;
	nextX += currentDirection.x;
	nextZ += currentDirection.z;
}

void Ghost::TargetRandom() {
	targetX = rand() % World::Instance().WIDTH;
	targetZ = rand() % World::Instance().HEIGHT;
}

void Ghost::TargetCorner() {
	targetX = cornerX;
	targetZ = cornerZ;
}

void Ghost::PickTarget() {}