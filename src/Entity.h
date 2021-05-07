#pragma once
#include <memory>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Model.h"
#include "Render/Shader.h"

class Entity
{
public:
	Entity(std::shared_ptr<Model> objectModel);
	void Draw(std::shared_ptr<Shader> shader);
	glm::vec3 GetPosition();
	void Translate(glm::vec3 translation);
	void Rotate(float angle, glm::vec3 direction);
	void Scale(glm::vec3 factor);

private:
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	std::shared_ptr<Model> model;

	void UpdateNormalMatrix();
};