#pragma once
#include "Sprite.h"
#include "Text.h"
#include "../GameObjects/Points.h"
#include "../GameObjects/Pacman.h"
#include "../Render/Shader.h"
#include "../Game.h"
#include "../GameObjects/Points.h"

class UI
{
public:
	UI(std::shared_ptr<Pacman> player, std::shared_ptr<Points> points);
	void Draw() const;
	void DrawGameBegin() const;
	void DrawReady() const;

private:
	const float OFFSET = 15.0f;
	const float FONT_SCALE = 78.0f;
	glm::mat4 projection;
	std::unique_ptr<Text> textRenderer;
	std::unique_ptr<Sprite> pacmanLive;
	std::unique_ptr<Sprite> cherry;
	std::shared_ptr<Pacman> player;
	std::shared_ptr<Points> points;

	void DrawScoreAndLevel() const;
	void DrawHealth() const;
	void DrawCherry() const;
};