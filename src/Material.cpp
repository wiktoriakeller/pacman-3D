#include "Material.h"

Material::	Material(std::vector<std::shared_ptr<Texture>> diffuseTextures, std::vector<std::shared_ptr<Texture>> specularTextures,
		std::vector<std::shared_ptr<Texture>> normalTextures, std::vector<std::shared_ptr<Texture>> heightTextures, float shininess) :
	diffuseMaps(diffuseTextures), 
	specularMaps(specularTextures),
	normalMaps(normalTextures), 
	heightMaps(heightTextures), 
	shininess(shininess) { }

void Material::SendMaterialToShader(std::shared_ptr<Shader> shader) const {
	int j = 0;


	for(int i = 0; i < diffuseMaps.size(); i++) {
		shader->SetUniform("uMaterial.diffuseMap" + std::to_string(i + 1), j);
		diffuseMaps[i]->Bind(j);
		j++;
	}

	for(int i = 0; i < specularMaps.size(); i++) {
		shader->SetUniform("uMaterial.specularMap" + std::to_string(i + 1), j);
		specularMaps[i]->Bind(j);
		j++;
	}

	for(int i = 0; i < normalMaps.size(); i++) {
		shader->SetUniform("uMaterial.normalMap" + std::to_string(i + 1), j);
		normalMaps[i]->Bind(j);
		j++;
	}

	for(int i = 0; i < heightMaps.size(); i++) {
		shader->SetUniform("uMaterial.heightMap" + std::to_string(i + 1), j);
		heightMaps[i]->Bind(j);
		j++;
	}

	shader->SetUniform("uMaterial.shininess", shininess);
}