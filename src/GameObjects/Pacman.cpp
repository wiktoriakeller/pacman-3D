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
	startPosition.x -= 0.6f;
	currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	wantedDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	stopped = true;
	powerPillEffect = false;
	Rotate(-30, glm::vec3(1.0f, 0.0f, 0.0f));
	Scale(glm::vec3(0.8f, 0.8f, 0.8f));
	SetPosition(startPosition);

	//animation
	animationRotationAngle = 15.0f;
	animationRotationSpeed = -80.0f;
	topPart = glm::mat4(1.0f);
	bottomPart = glm::mat4(1.0f);
}

void Pacman::Update(float deltaTime) {
	powerPillEffect = false;
	HandleInput();
	EvaluateDirection();
	Animate(deltaTime);
	Move(deltaTime);
}

void Pacman::Reset() {
	currentDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	wantedDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	nextX = startX;
	nextZ = startZ;
	stopped = true;
	shouldRotate = true;
	Move(0.0f);
	currentDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	SetPosition(startPosition);
}

void Pacman::Draw(std::shared_ptr<Shader> shader) {
	//top part
	glm::mat4 modelMatrixTop = modelMatrix * topPart;
	model->SendMaterialToShader(0, shader);
	shader->SetUniform("uModel", modelMatrixTop);
	shader->SetUniform("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(modelMatrixTop))));
	model->DrawMesh(0);
	model->UnbindMaterial(0);

	//bottom part
	glm::mat4 modelMatrixBottom = modelMatrix * bottomPart;
	model->SendMaterialToShader(1, shader);
	shader->SetUniform("uModel", modelMatrixBottom);
	shader->SetUniform("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(modelMatrixBottom))));
	model->DrawMesh(1);
	model->UnbindMaterial(1);
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

void Pacman::Animate(float deltaTime) {
	float angle = deltaTime * animationRotationSpeed;

	if (animationRotationAngle + angle >= maxRotation) {
		animationRotationSpeed *= -1;
		angle = maxRotation - animationRotationAngle;

	}
	else if (animationRotationAngle + angle < minRotation) {
		animationRotationSpeed *= -1;
		angle = animationRotationAngle;
	}

	animationRotationAngle += angle;

	topPart = glm::rotate(topPart, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
	bottomPart = glm::rotate(bottomPart, glm::radians(-angle), glm::vec3(1.0f, 0.0f, 0.0f));
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

	if (element == MapElement::Point || element == MapElement::Power || element == MapElement::Cherry) {
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