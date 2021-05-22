#pragma once
#include "Sprite.h"
#include "Text.h"
#include "../GameObjects/Points.h"
#include "../GameObjects/Pacman.h"
#include "../Render/Shader.h"
#include "../Game.h"

class UI
{
public:
	UI();
	void Draw() const;

private:
	const float OFFSET = 15.0f;
	glm::mat4 projection;
	std::unique_ptr<Text> textRenderer;
	std::unique_ptr<Sprite> pacmanLive;

	void DrawScoreAndLevel() const;
	void DrawHealth() const;
};