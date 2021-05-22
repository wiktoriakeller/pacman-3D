#pragma once
#include "Ghost.h"
#include <algorithm>

class Pinky : public Ghost
{
public:
	Pinky(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman);
	void PickTarget() override;
private:
	const int START_X = 14;
	const int START_Z = 17;
	void CenterInHouse();
};

