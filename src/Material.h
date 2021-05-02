#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>
#include <vector>
#include <assimp/scene.h>

#include "Render/Shader.h"
#include "Render/Texture.h"

class Material
{
public:
	Material(float shininess);
	Material(const std::vector<unsigned int>& diffuseTextures, const std::vector<unsigned int>& specularTextures,
		const std::vector<unsigned int>& normalTextures, float shininess);

	void SendMaterialToShader(std::shared_ptr<Shader> shader, const std::vector<std::unique_ptr<Texture>>& textures) const;
	void AddNewMapIndex(unsigned int index, const aiTextureType& type);

private:
	void SendMaps(std::shared_ptr<Shader> shader, const std::vector<unsigned int>& maps, const std::string& name, int& index,
		const std::vector<std::unique_ptr<Texture>>& textures) const;

	std::vector<unsigned int> diffuseMaps;
	std::vector<unsigned int> specularMaps;
	std::vector<unsigned int> normalMaps;
    float shininess;
};