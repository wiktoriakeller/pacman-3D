#pragma once
#include <memory>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "../Model.h"
#include "../Render/Shader.h"

class Entity
{
public:
	Entity(std::unique_ptr<Model> objectModel);
	virtual void Draw(std::shared_ptr<Shader> shader);
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 position);
	void Translate(glm::vec3 translation);
	void Rotate(float angle, glm::vec3 direction);
	void Scale(glm::vec3 factor);
	virtual void Update(float deltaTime);

private:
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	std::unique_ptr<Model> model;

	void UpdateNormalMatrix();
};