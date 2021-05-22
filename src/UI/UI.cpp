#include "UI.h"

UI::UI(unsigned int windowWidth, unsigned int windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight)  {
	projection = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight);
	pacmanLive = std::make_unique<Sprite>("Resources/Sprites/pacman.png");
	pacmanLive->SetScale(glm::vec2(30.0f, 30.0f));
	textRenderer = std::make_unique<Text>("Resources/Fonts/arial.ttf", 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), 48.0f);
}

void UI::Draw() {
	DrawHealth();
	DrawScoreAndLevel();
}

void UI::DrawHealth() {
	unsigned int lives = Pacman::GetLives();

	shaderMap["spriteShader"]->Use();
	shaderMap["spriteShader"]->SetUniform("uProjection", projection);

	for (int i = 0; i < lives; i++) {
		pacmanLive->SetPosition(glm::vec2(windowWidth - (i + 1) * (OFFSET + pacmanLive->GetSize().x), OFFSET));
		pacmanLive->Draw(shaderMap["spriteShader"]);
	}
}

void UI::DrawScoreAndLevel() {
	shaderMap["textShader"]->Use();
	shaderMap["textShader"]->SetUniform("uProjection", projection);
	textRenderer->Draw(shaderMap["textShader"], "Score: " + std::to_string(Points::GetScore()), OFFSET, OFFSET);
	textRenderer->Draw(shaderMap["textShader"], "Level: 0", OFFSET, OFFSET + 2 * OFFSET);
}