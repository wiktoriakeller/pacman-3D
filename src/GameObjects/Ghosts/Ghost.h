#pragma once
#include <cstdlib>
#include "../Moveable.h"

enum class State : unsigned char
{
	House = 0, Scatter, Chase, Frightened, Leaving
};

enum class LeavingState : unsigned char
{
	CenterX = 0, CenterZ, BackToNormal
};

class Ghost : public Moveable
{
public:
	Ghost(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman);
	void Update(float deltaTime) override;
	virtual void PickTarget() = 0;
	void Reset() override;

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
	std::shared_ptr<Moveable> pacman;

	void PickPath(bool canTurnAround);
	void TargetCorner();
	void ChangeState(State newState);

private:
	float baseSpeed;
	float chaseTime;
	float scatterTime;
	float frightenedTime;
	float timer;
	LeavingState leavingState;
	std::vector<glm::vec3> directions = { 
		glm::vec3(0.0f,0.0f,-1.0f),			//left
		glm::vec3(1.0f,0.0f,0.0f),			//right
		glm::vec3(-1.0f,0.0f,0.0f),			//up
		glm::vec3(0.0f,0.0f,1.0f)			//down
	};	

	void LeaveHouse();
	void TargetRandom();
	void UpdateState();
	void HandleMovement(float deltaTime);
};