#pragma once
#include "Ghost.h"

class Clyde : public Ghost
{
public:
	Clyde(std::unique_ptr<Model> model, std::shared_ptr<Pacman> pacman);
	void PickTarget() override;
};