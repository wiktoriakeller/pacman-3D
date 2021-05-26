#pragma once
#include "Ghost.h"

class Blinky : public Ghost
{
public:
	Blinky(std::unique_ptr<Model> model, std::shared_ptr<Pacman> pacman, bool createPointLight);
	void PickTarget() override;
};