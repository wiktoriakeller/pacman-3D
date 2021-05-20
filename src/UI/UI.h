#pragma once
#include "Sprite.h"
#include "Text.h"
#include "../GameObjects/Points.h"
#include "../GameObjects/Pacman.h"
#include "../Render/Shader.h"

class UI
{
public:
	UI(unsigned int windowWidth, unsigned int windowHeight);
	void Draw();

private:
	const float OFFSET = 15.0f;
	unsigned int windowWidth;
	unsigned int windowHeight;
	glm::mat4 projection;
	std::unique_ptr<Text> textRenderer;
	std::unique_ptr<Sprite> pacmanLive;

	void DrawScore();
	void DrawHealth();
};