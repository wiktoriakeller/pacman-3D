#include "UI.h"

UI::UI(std::shared_ptr<Pacman> player, std::shared_ptr<Points> points) : player(player), points(points) {
	projection = glm::ortho(0.0f, (float)Game::WINDOW_WIDTH, 0.0f, (float)Game::WINDOW_HEIGHT);
	pacmanLive = std::make_unique<Sprite>("Resources/Sprites/pacman.png");
	cherry = std::make_unique<Sprite>("Resources/Sprites/cherry.png");
	pacmanLive->SetScale(glm::vec2(35.0f, 35.0f));
	cherry->SetScale(glm::vec2(45.0f, 45.0f));
	textRenderer = std::make_unique<Text>("Resources/Fonts/minecraft.ttf", FONT_SCALE, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 48);
}

void UI::Draw() const {
	DrawHealth();
	DrawCherry();
	DrawScoreAndLevel();
}

void UI::DrawGameBegin() const {
	std::string text = "PRESS ANY KEY TO START";
	float fontModifier = 0.8f;
	textRenderer->SetFontColor(glm::vec4(1.0f, 1.0f, 1.0f, sin(glfwGetTime() * 2.5f) / 2.0f + 0.4f));
	textRenderer->SetFontScale(fontModifier);
	shaderMap["textShader"]->Use();
	shaderMap["textShader"]->SetUniform("uProjection", projection);
	textRenderer->Draw(shaderMap["textShader"], text, Game::WINDOW_WIDTH / 2.0f - (text.size() * textRenderer->GetWidth() / 2),
		Game::WINDOW_HEIGHT / 2.0f);
}

void UI::DrawReady() const {
	std::string text = "READY!";
	float fontModifier = 0.8f;
	textRenderer->SetFontColor(glm::vec4(1.0f, 1.0f, 0.0f, sin(glfwGetTime() * 5.5f) / 2.0f + 0.4f));
	textRenderer->SetFontScale(fontModifier);
	shaderMap["textShader"]->Use();
	shaderMap["textShader"]->SetUniform("uProjection", projection);
	textRenderer->Draw(shaderMap["textShader"], text, Game::WINDOW_WIDTH / 2.0f - (text.size() * textRenderer->GetWidth() / 2),
		Game::WINDOW_HEIGHT / 2.0f);
}

void UI::DrawHealth() const {
	int lives = player->GetLives();

	shaderMap["spriteShader"]->Use();
	shaderMap["spriteShader"]->SetUniform("uProjection", projection);

	for (int i = 0; i < lives; i++) {
		pacmanLive->SetPosition(glm::vec2(Game::WINDOW_WIDTH - (i + 1) * (OFFSET + pacmanLive->GetSize().x) + i, OFFSET));
		pacmanLive->Draw(shaderMap["spriteShader"]);
	}
}

void UI::DrawCherry() const {
	if (!points->GetIsCherryPicked()) {
		shaderMap["spriteShader"]->Use();
		shaderMap["spriteShader"]->SetUniform("uProjection", projection);
		cherry->SetPosition(glm::vec2(Game::WINDOW_WIDTH - 2 * (OFFSET + pacmanLive->GetSize().x) - cherry->GetSize().x - 2 * OFFSET + 1, OFFSET - 5.0f));
		cherry->Draw(shaderMap["spriteShader"]);
	}
}

void UI::DrawScoreAndLevel() const {
	float fontModifier = 0.55f;
	textRenderer->SetFontColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	textRenderer->SetFontScale(fontModifier);
	shaderMap["textShader"]->Use();
	shaderMap["textShader"]->SetUniform("uProjection", projection);
	textRenderer->Draw(shaderMap["textShader"], "Score: " + std::to_string(points->GetScore()), OFFSET, OFFSET);
	textRenderer->Draw(shaderMap["textShader"], "Level: " + std::to_string(points->GetLevel() + 1), OFFSET, OFFSET + 2.5f * OFFSET);
}