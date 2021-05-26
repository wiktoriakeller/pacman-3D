#include "UI.h"

UI::UI(std::shared_ptr<Pacman> player, std::shared_ptr<Points> points) : player(player), points(points) {
	projection = glm::ortho(0.0f, (float)Game::WINDOW_WIDTH, 0.0f, (float)Game::WINDOW_HEIGHT);
	pacmanLive = std::make_unique<Sprite>("Resources/Sprites/pacman.png");
	pacmanLive->SetScale(glm::vec2(30.0f, 30.0f));
	textRenderer = std::make_unique<Text>("Resources/Fonts/arial.ttf", 0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 48.0f);
	waitingTextRenderer = std::make_unique<Text>("Resources/Fonts/arial.ttf", 0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 72.0f);
}

void UI::Draw() const {
	DrawHealth();
	DrawScoreAndLevel();
}

void UI::DrawGameBegin() const {
	std::string text = "PRESS ANY KEY TO START";
	waitingTextRenderer->SetFontColor(glm::vec4(1.0f, 1.0f, 1.0f, sin(glfwGetTime() * 2) / 2.0f + 0.4f));
	shaderMap["textShader"]->Use();
	shaderMap["textShader"]->SetUniform("uProjection", projection);
	waitingTextRenderer->Draw(shaderMap["textShader"], text, Game::WINDOW_WIDTH / 2.0f - Game::WINDOW_WIDTH / 3.56f,
		Game::WINDOW_HEIGHT / 2.0f + Game::WINDOW_HEIGHT / 120.0f);
}

void UI::DrawHealth() const {
	int lives = player->GetLives();

	shaderMap["spriteShader"]->Use();
	shaderMap["spriteShader"]->SetUniform("uProjection", projection);

	for (int i = 0; i < lives; i++) {
		pacmanLive->SetPosition(glm::vec2(Game::WINDOW_WIDTH - (i + 1) * (OFFSET + pacmanLive->GetSize().x), OFFSET));
		pacmanLive->Draw(shaderMap["spriteShader"]);
	}
}

void UI::DrawScoreAndLevel() const {
	shaderMap["textShader"]->Use();
	shaderMap["textShader"]->SetUniform("uProjection", projection);
	textRenderer->Draw(shaderMap["textShader"], "Score: " + std::to_string(points->GetScore()), OFFSET, OFFSET);
	textRenderer->Draw(shaderMap["textShader"], "Level: " + std::to_string(points->GetLevel()), OFFSET, OFFSET + 2 * OFFSET);
}