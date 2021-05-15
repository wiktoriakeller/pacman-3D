#include "Points.h"

Points::Points(std::unique_ptr<Model> model) : Entity(std::move(model)) { 
    Scale(glm::vec3(0.25f, 0.25f, 0.25f));
}

void Points::Draw(std::shared_ptr<Shader> shader) {
    for (int y = 0; y < World::HEIGHT; y++) {
        for (int x = 0; x < World::WIDTH; x++) {
            if (World::Instance().GetMapElement(x, y) == MapElement::Point || World::Instance().GetMapElement(x, y) == MapElement::Power) {
                SetPosition(World::Instance().GetPosition(x, y));
                shader->SetUniform("uModel", modelMatrix);
                shader->SetUniform("uNormalMatrix", normalMatrix);
                model->Draw(shader);
            }
        }
    }
}