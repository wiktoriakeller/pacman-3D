#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

#include "Render/Shader.h"
#include "Render/Texture.h"

class Material
{
public:
	Material(std::vector<std::shared_ptr<Texture>> diffuseTextures, std::vector<std::shared_ptr<Texture>> specularTextures,
		std::vector<std::shared_ptr<Texture>> normalTextures, std::vector<std::shared_ptr<Texture>> heightTextures, float shininess);

	void SendMaterialToShader(std::shared_ptr<Shader> shader) const;

private:
	std::vector<std::shared_ptr<Texture>> diffuseMaps;
	std::vector<std::shared_ptr<Texture>> specularMaps;
	std::vector<std::shared_ptr<Texture>> normalMaps;
	std::vector<std::shared_ptr<Texture>> heightMaps;

    float shininess;
};