#include "Game.h"

bool Game::isGameOver = false;

void Game::SetIsGameOver(bool gameOver) {
	isGameOver = gameOver;
}

bool Game::GetIsGameOver() {
	return isGameOver;
}

void Game::Reset() {
	if (isGameOver) {
		isGameOver = false;
	}
}