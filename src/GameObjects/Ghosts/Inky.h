#pragma once
#include "Ghost.h"

class Inky : public Ghost
{
public:
	Inky(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman, std::shared_ptr<Moveable> blinky);
	void PickTarget() override;
private:
	const int START_X = 15;
	const int START_Z = 17;
	std::shared_ptr<Moveable> blinky;
	void CenterInHouse();
};			  

