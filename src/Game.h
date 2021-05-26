#pragma once
#include "Input/KeyInput.h"

class Game
{
public:
	static const unsigned int WINDOW_WIDTH = 800;
	static const unsigned int WINDOW_HEIGHT = 600;

	static void SetIsGameOver(bool gameOver);
	static bool GetIsGameOver();
	void Reset();

private:
	static bool isGameOver;
};