#include "Points.h"

Points::Points(std::unique_ptr<Model> model, std::unique_ptr<Entity> cherry, bool createPointLight) : Entity(std::move(model), createPointLight), 
    cherry(std::move(cherry)) {
    Scale(glm::vec3(0.25f, 0.25f, 0.25f));
    pointsLeft = START_POINTS;
    level = 0;
    score = 0;
    ghostScoreMultiplier = 1;
    cherrySpawned = false;
    cherryTimer = 0.0f;
    cherryDisappearTime = 30.0f;
    this->cherry->SetScale(glm::vec3(0.35f, 0.35f, 0.35f));
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
            else if (World::Instance().GetMapElement(x, y) == MapElement::Cherry) {
                glm::vec3 cherryPos = World::Instance().GetPosition(x, y);
                cherryPos.x -= 0.4f;
                cherry->SetPosition(cherryPos);
                cherry->Draw(shader);
            }
        }
    }
}

void Points::AddPoints(MapElement element, int x, int z) {
    if (element == MapElement::Point) {
        score += POINT_SCORE;
        World::Instance().SetMapElement(x, z, MapElement::MissingPoint);
        pointsLeft -= 1;
    }
    else if (element == MapElement::Power) {
        score += POWER_SCORE;
        World::Instance().SetMapElement(x, z, MapElement::MissingPower);
        pointsLeft -= 1;
    }
    else if (element == MapElement::Ghost) {
        score += (GHOST_KILL_SCORE * ghostScoreMultiplier);
        ghostScoreMultiplier++;
    }
    else if (element == MapElement::Cherry) {
        score += CHERRY_SCORE;
        World::Instance().SetMapElement(x, z, MapElement::MissingPoint);
    }
}

void Points::ResetGhostScoreMultiplier() {
    ghostScoreMultiplier = 1;
}

void Points::Reset() {
    if (pointsLeft == 0 || Game::GetIsGameOver()) {
        for (int y = 0; y < World::HEIGHT; y++) {
            for (int x = 0; x < World::WIDTH; x++) {
                if (World::Instance().GetMapElement(x, y) == MapElement::MissingPoint) {
                    World::Instance().SetMapElement(x, y, MapElement::Point);
                }
                else if (World::Instance().GetMapElement(x, y) == MapElement::MissingPower) {
                    World::Instance().SetMapElement(x, y, MapElement::Power);
                }
                else if (World::Instance().GetMapElement(x, y) == MapElement::Cherry) {
                    World::Instance().SetMapElement(x, y, MapElement::Point);
                }
            }
        }

        pointsLeft = START_POINTS;

        if (Game::GetIsGameOver()) {
            level = 0;
            score = 0;
        }
        else {
            level++;
        }
        
        cherryTimer = 0.0f;
        cherrySpawned = false;
    }
    else if (cherrySpawned) {
        World::Instance().SetMapElement(cherryX, cherryZ, MapElement::MissingPoint);
    }
}

unsigned int Points::GetScore() {
    return score;
}

unsigned int Points::GetLevel() {
    return level;
}

unsigned int Points::GetPointsLeft(){
    return pointsLeft;
}

bool Points::GetIsCherrySpawned() {
    return cherrySpawned;
}

void Points::Update(float deltaTime) {
    if (cherrySpawned) {
        cherryTimer += deltaTime;
    }

    if (!cherrySpawned && (START_POINTS - pointsLeft) >= POINTS_TO_SPAWN_CHERRY) {
        std::vector<std::pair<int, int>> positions;
        for (int y = 0; y < World::HEIGHT; y++) {
            for (int x = 0; x < World::WIDTH; x++) {
                if (World::Instance().GetMapElement(x, y) == MapElement::MissingPoint) {
                    positions.push_back(std::pair(x, y));
                }
            }
        }

        int index = rand() % positions.size();
        cherryX = positions[index].first;
        cherryZ = positions[index].second;
        World::Instance().SetMapElement(cherryX, cherryZ, MapElement::Cherry);
        cherrySpawned = true;
    }

    if (cherrySpawned && cherryTimer > cherryDisappearTime) {
        World::Instance().SetMapElement(cherryX, cherryZ, MapElement::MissingPoint);
    }
}