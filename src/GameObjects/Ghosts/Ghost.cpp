#include "Ghost.h"

Ghost::Ghost(std::unique_ptr<Model> model, std::shared_ptr<Pacman> pacman, bool createPointLight) : 
	Moveable(std::move(model), createPointLight), pacman(pacman) {
	timer = 0.0f;
	frightenedTimer = 0.0f;
	chaseTime = 52.0f;
	scatterTime = 8.0f;
	frightenedTime = 8.0f;
	baseSpeed = 5.0f;
	blinkingTimer = 0;
	speed = baseSpeed;
	isFrightened = false;
	leavingState = LeavingState::CenterX;
	returningState = ReturningState::GoingBack;
	currentState = State::House;
	startState = currentState;
	Scale(glm::vec3(0.7f, 0.7f, 0.7f));
	Rotate(90, glm::vec3(0.0f, 1.0f, 0.0f));
	frightenedColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	this->model->ChangeMeshMaterialDiffuse(MAT_COLOR_INDEX, frightenedColor);
}

void Ghost::Update(float deltaTime) {
	timer += deltaTime;
	
	if (isFrightened) {
		frightenedTimer += deltaTime;
	}

	if (isFrightened && !IsReturning() && frightenedTimer < frightenedTime && frightenedTimer >= (frightenedTime - 1.5f)) {
		blinkingTimer++;
		int reminder = blinkingTimer % 30;
		if (reminder < 15) {
			model->ChangeMeshMaterialDiffuse(MAT_COLOR_INDEX, frightenedColor);
		}
		else {
			model->ChangeMeshMaterialDiffuse(MAT_COLOR_INDEX, glm::vec4(1.0f));
		}
	}

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

	case State::Returning:
		EnterHouse();
		break;

	default:
		if (SnapToGrid()) {
			PickPath();
		}
		break;
	}

	if (World::Instance().GetMapElement(nextX, nextZ) == MapElement::Tunnel) {
		CrossTunnel();
	}

	Move(deltaTime);
}

void Ghost::UpdateState() {
	if(pacman->GetPowerPillEffect()) {
		Frighten();
	}

	if (isFrightened && frightenedTimer > frightenedTime) {
		isFrightened = false;
		blinkingTimer = 0.0f;
		frightenedTimer = 0.0f;

		if (currentState != State::Returning && currentState != State::House && currentState != State::Leaving) {
			ChangeBaseState(State::Chase);
		}
		
		if (currentState != State::Returning) {
			model->UseMeshMaterialDiffuseColor(MAT_COLOR_INDEX, false);
			model->ChangeMeshMaterialDiffuse(MAT_COLOR_INDEX, frightenedColor);
		}
	}

	switch (currentState) {
	case State::House:
		speed = 0.0f;
		if (timer > houseTime) {
			speed = baseSpeed;
			ChangeBaseState(State::Leaving);
		}
		break;

	case State::Scatter:
		if (isFrightened) {
			TargetRandom();
		}
		else {
			TargetCorner();
		}

		if (timer > scatterTime) {
			ChangeBaseState(State::Chase);
		}
		break;

	case State::Chase:
		if (isFrightened) {
			TargetRandom();
		}
		else {
			PickTarget();
		}

		if (timer > chaseTime) {
			ChangeBaseState(State::Scatter);
		}
		break;

	case State::Returning:
		if (returningState == ReturningState::GoingBack && GetCoordinates().x == targetX && GetCoordinates().z == targetZ) {
			SetPosition(glm::vec3(World::Instance().WorldCenter.x, GetPosition().y, GetPosition().z));
			returningState = ReturningState::CenterZ;
			speed = baseSpeed;
		}
		break;

	default:
		break;
	}
}

void Ghost::ChangeBaseState(State newState) {
	currentState = newState;
	timer = 0.0f;
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
			SetPosition(glm::vec3(World::Instance().WorldCenter.x, GetPosition().y, World::Instance().WorldCenter.z));
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
		ChangeBaseState(State::Scatter);
		UpdateState();
		PickPath();
		break;

	default:
		break;
	}
}

void Ghost::EnterHouse() {
	shouldRotate = true;

	switch (returningState)
	{
	case ReturningState::GoingBack:
		if (SnapToGrid()) {
			PickPath();
		}
		break;

	case ReturningState::CenterZ:
		if (!World::Instance().ArePositionsEqual(GetPosition(), World::Instance().WorldCenter)) {
			currentDirection = glm::vec3(0.0f, 0.0f, 1.0f);
		}
		else {
			SetPosition(glm::vec3(World::Instance().WorldCenter.x, GetPosition().y, World::Instance().WorldCenter.z));
			returningState = ReturningState::StartPosition;
		}
		break;

	case ReturningState::StartPosition:
		if (!World::Instance().ArePositionsEqual(GetPosition(), glm::vec3(startPosition.x, GetPosition().y, GetPosition().z)) 
			&& (startZ == HOUSE_CENTER_Z || startX != HOUSE_CENTER_X)) {
			float direction = 1.0f;
			if (startPosition.x < World::Instance().WorldCenter.x) {
				direction = -1.0f;
			}
			currentDirection = glm::vec3(direction, 0.0f, 0.0f);
		}
		else {
			model->UseMeshMaterialDiffuseColor(MAT_COLOR_INDEX, false);
			model->ChangeMeshMaterialDiffuse(MAT_COLOR_INDEX, frightenedColor);
			isFrightened = false;
			frightenedTimer = 0.0f;
			ChangeBaseState(State::Leaving);

			if (startZ != HOUSE_CENTER_Z || startX == HOUSE_CENTER_X) {
				leavingState = LeavingState::CenterZ;
			}
			else {
				SetPosition(startPosition);
				leavingState = LeavingState::CenterX;
			}
		}
		break;

	default:
		break;
	}
}

void Ghost::PickPath() {
	float minDistance = std::numeric_limits<float>::max();
	int bestDirectionIndex = 0;
	
	for (int i = 0; i < directions.size(); i++) {
		if (currentDirection.x != 0 && directions[i].x == currentDirection.x * -1) {
			continue;
		}
		else if (currentDirection.z != 0 && directions[i].z == currentDirection.z * -1) {
			continue;
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
	nextX += currentDirection.x;
	nextZ += currentDirection.z;
	shouldRotate = true;
}

void Ghost::TargetRandom() {
	targetX = rand() % World::Instance().WIDTH;
	targetZ = rand() % World::Instance().HEIGHT;
}

void Ghost::TargetCorner() {
	targetX = cornerX;
	targetZ = cornerZ;
}

void Ghost::Reset() {
	SetPosition(startPosition);
	model->ChangeMeshMaterialDiffuse(MAT_COLOR_INDEX, frightenedColor);
	model->UseMeshMaterialDiffuseColor(MAT_COLOR_INDEX, false);
	currentDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	nextX = startX;
	nextZ = startZ;
	speed = baseSpeed;
	timer = 0.0f;
	frightenedTimer = 0.0f;
	currentState = startState;
	leavingState = LeavingState::CenterX;
	returningState = ReturningState::GoingBack;
	isFrightened = false;
	shouldRotate = true;
	Move(0.0f);
}

void Ghost::ReturnToHouse() {
	targetX = HOUSE_CENTER_X;
	targetZ = HOUSE_CENTER_Z - 3;
	speed = 2.5f * baseSpeed;
	model->ChangeMeshMaterialDiffuse(MAT_COLOR_INDEX, returningColor);
	returningState = ReturningState::GoingBack;
	ChangeBaseState(State::Returning);
}

void Ghost::Frighten() {
	if (currentState != State::Returning) {
		isFrightened = true;
		frightenedTimer = 0.0f;
		model->UseMeshMaterialDiffuseColor(MAT_COLOR_INDEX, true);
		model->ChangeMeshMaterialDiffuse(MAT_COLOR_INDEX, frightenedColor);
		glm::vec3 reversedDirection = -currentDirection;

		if (currentDirection.x != 0 && CanMakeMove(nextX + reversedDirection.x, nextZ)
			&& (currentState == State::Chase || currentState == State::Scatter)) {
			currentDirection = reversedDirection;
			nextX = nextX + reversedDirection.x;
			shouldRotate = true;
		}
		else if (currentDirection.z != 0 && CanMakeMove(nextX, nextZ + reversedDirection.z)
			&& (currentState == State::Chase || currentState == State::Scatter)) {
			currentDirection = reversedDirection;
			nextZ = nextZ + reversedDirection.z;
			shouldRotate = true;
		}
	}
}

bool Ghost::IsFrightenedOrReturning() {
	if(isFrightened || currentState == State::Returning) {
		return true;
	}
	return false;
}

bool Ghost::IsReturning() {
	if (currentState == State::Returning) {
		return true;
	}
	return false;
}

bool Ghost::IsFrightened() {
	return isFrightened;
}

float Ghost::GetFrightenedTimer() {
	return frightenedTimer;
}

float Ghost::GetFrightenedTime() {
	return frightenedTime;
}