#pragma once
#include "Ghost.h"

class Clyde : public Ghost
{
public:
	Clyde(std::unique_ptr<Model> model, std::shared_ptr<Moveable> pacman);
	void PickTarget() override;
private:
	const int START_X = 12;
	const int START_Z = 17;
};

