#pragma once
#include "Ghost.h"

class Inky : public Ghost
{
public:
	Inky(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman, std::shared_ptr<Moveable> blinky);
	void PickTarget() override;
private:
	std::shared_ptr<Moveable> blinky;
};			  

