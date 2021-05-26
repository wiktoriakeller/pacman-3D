#pragma once
#include <cstdlib>
#include "../Pacman.h"

enum class State : unsigned char
{
	House = 0, Scatter, Chase, Leaving, Returning
};

enum class LeavingState : unsigned char
{
	CenterX = 0, CenterZ, BackToNormal
};

enum class ReturningState : unsigned char
{
	GoingBack = 0, CenterZ, StartPosition
};

class Ghost : public Moveable
{
public:
	Ghost(std::unique_ptr<Model> model, std::shared_ptr<Pacman> pacman);
	void Update(float deltaTime) override;
	virtual void PickTarget() = 0;
	void Reset() override;
	void ReturnToHouse();
	bool IsFrightened();

protected:
	const int HOUSE_CENTER_X = 14;
	const int HOUSE_CENTER_Z = 17;
	int targetX;
	int targetZ;
	int cornerX;
	int cornerZ;
	float houseTime;
	State currentState;
	State startState;
	std::shared_ptr<Pacman> pacman;

	void PickPath();
	void TargetCorner();
	void ChangeBaseState(State newState);
	
private:
	const unsigned int MAT_COLOR_INDEX = 1;
	float timer;
	float frightenedTimer;
	float baseSpeed;
	float chaseTime;
	float scatterTime;
	float frightenedTime;
	bool isFrightened;
	LeavingState leavingState;
	ReturningState returningState;
	std::vector<glm::vec3> directions = { 
		glm::vec3(0.0f, 0.0f, -1.0f),			//left
		glm::vec3(1.0f, 0.0f, 0.0f),			//right
		glm::vec3(-1.0f, 0.0f, 0.0f),			//up
		glm::vec3(0.0f, 0.0f, 1.0f)			//down
	};	

	void Frighten();
	void LeaveHouse();
	void EnterHouse();
	void TargetRandom();
	void UpdateState();
	void HandleMovement(float deltaTime);
};