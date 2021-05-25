#pragma once
#include "Ghost.h"
#include <algorithm>

class Pinky : public Ghost
{
public:
	Pinky(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman);
	void PickTarget() override;
};

