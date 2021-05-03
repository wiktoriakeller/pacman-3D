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
	Material(glm::vec3 specularColor, float shininess);
	Material(glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess);

	void SendToShader(std::shared_ptr<Shader> shader, const std::vector<std::unique_ptr<Texture>>& textures) const;
	void UnbindMaterial(const std::vector<std::unique_ptr<Texture>>& textures) const;
	void AddNewMapIndex(unsigned int index, const aiTextureType& type);
	void SetDiffuseColor(glm::vec3 color);
	void SetSpecularColor(glm::vec3 color);
	void SetShininess(float newShininess);
	void SetMeshIndex(int index);
	int GetMeshIndex() const;
	void SetAssimpMaterialIndex(int index);
	int GetAssimpMaterialIndex() const;

private:
	void SendMaps(std::shared_ptr<Shader> shader, const std::vector<unsigned int>& maps, const std::string& name, int& index,
		const std::vector<std::unique_ptr<Texture>>& textures) const;
	void UnbindMaps(const std::vector<unsigned int>& maps, const std::string& name,
		const std::vector<std::unique_ptr<Texture>>& textures) const;
	
	float shininess;
	int meshIndex;
	int assimpMaterialIndex;

	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	std::vector<unsigned int> diffuseMaps;
	std::vector<unsigned int> specularMaps;
	std::vector<unsigned int> normalMaps;
};