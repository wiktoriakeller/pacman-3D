#include "Points.h"

unsigned int Points::score = 0;

Points::Points(std::unique_ptr<Model> model) : Entity(std::move(model)) { 
    Scale(glm::vec3(0.25f, 0.25f, 0.25f));
    pointsLeft = World::START_POINTS;
}

void Points::Draw(std::shared_ptr<Shader> shader) {
    for (int y = 0; y < World::HEIGHT; y++) {
        for (int x = 0; x < World::WIDTH; x++) {
            if (World::Instance().GetMapElement(x, y) == MapElement::Point) {
                SetPosition(World::Instance().GetPosition(x, y));
                shader->SetUniform("uModel", modelMatrix);
                shader->SetUniform("uNormalMatrix", normalMatrix);
                model->Draw(shader);
            }
            else if (World::Instance().GetMapElement(x, y) == MapElement::Power) {
                SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
                SetPosition(World::Instance().GetPosition(x, y));
                shader->SetUniform("uModel", modelMatrix);
                shader->SetUniform("uNormalMatrix", normalMatrix);
                model->Draw(shader);
                SetScale(glm::vec3(0.25f, 0.25f, 0.25f));
            }
        }
    }
}

void Points::AddPoints(MapElement element) {
    if (element == MapElement::Point) {
        score += POINT_SCORE;
    }
    else if (element == MapElement::Power) {
        score += POWER_SCORE;
    }
    pointsLeft -= 1;
}

void Points::Reset() {
    for (int y = 0; y < World::HEIGHT; y++) {
        for (int x = 0; x < World::WIDTH; x++) {
            if (World::Instance().GetMapElement(x, y) == MapElement::MissingPoint) {
                World::Instance().SetMapElement(x, y, MapElement::Point);
            }
            else if (World::Instance().GetMapElement(x, y) == MapElement::MissingPower) {
                World::Instance().SetMapElement(x, y, MapElement::Power);
            }
        }
    }
    pointsLeft = World::START_POINTS;
}

unsigned int Points::GetScore() {
    return score;
}

int Points::GetPointsLeft(){
    return pointsLeft;
}
