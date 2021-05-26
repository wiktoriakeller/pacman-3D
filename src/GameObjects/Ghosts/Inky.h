#pragma once
#include "Ghost.h"

class Inky : public Ghost
{
public:
	Inky(std::unique_ptr<Model> model, std::shared_ptr<Pacman> pacman, std::shared_ptr<Moveable> blinky, bool createPointLight);
	void PickTarget() override;

private:
	std::shared_ptr<Moveable> blinky;
};			  