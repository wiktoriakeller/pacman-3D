#pragma once
#include "Ghost.h"

class Blinky : public Ghost
{
public:
	Blinky(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman);
	void PickTarget() override;
private:
	const int START_X = 14;
	const int START_Z = 14;
};

