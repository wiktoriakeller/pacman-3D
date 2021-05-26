#pragma once
#include <memory>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "../Model.h"
#include "../Render/Shader.h"
#include "../Light/PointLight.h"

class Entity
{
public:
	Entity(std::unique_ptr<Model> model, bool createPointLight);
	virtual void Draw(std::shared_ptr<Shader> shader);
	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 position);
	void Translate(glm::vec3 translation);
	void Rotate(float angle, glm::vec3 direction, bool castToRadians = true);
	void SetScale(glm::vec3 factor);
	void Scale(glm::vec3 factor);
	void SendLightToShader(std::shared_ptr<Shader> shader);
	virtual void Update(float deltaTime);
	virtual void Reset();

protected:
	std::unique_ptr<Model> model;
	std::unique_ptr<PointLight> pointLight;
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	void UpdateNormalMatrix();
};