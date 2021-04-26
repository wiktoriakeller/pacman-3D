#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <vector>

struct Material
{
	Material(const std::vector<unsigned int>& diffuseTextures, const std::vector<unsigned int>& specularTextures,
		const std::vector<unsigned int>& normalTextures, const std::vector<unsigned int>& heightTextures, float shininess) :
		diffuseMaps(diffuseTextures),
		specularMaps(specularTextures),
		normalMaps(normalTextures),
		heightMaps(heightTextures),
		shininess(shininess) { }

	std::vector<unsigned int> diffuseMaps;
	std::vector<unsigned int> specularMaps;
	std::vector<unsigned int> normalMaps;
	std::vector<unsigned int> heightMaps;
    float shininess;
};