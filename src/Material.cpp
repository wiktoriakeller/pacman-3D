#include "Material.h"

Material::Material(float shininess) : shininess(shininess) { }

Material::Material(const std::vector<unsigned int>& diffuseTextures, const std::vector<unsigned int>& specularTextures,
    const std::vector<unsigned int>& normalTextures, float shininess) :
    diffuseMaps(diffuseTextures),
    specularMaps(specularTextures),
    normalMaps(normalTextures),
    shininess(shininess) { }

void Material::SendMaterialToShader(std::shared_ptr<Shader> shader, const std::vector<std::unique_ptr<Texture>>& textures) const {
    int index = 0;

    SendMaps(shader, diffuseMaps, "uMaterial.diffuseMap", index, textures);
    SendMaps(shader, normalMaps, "uMaterial.normalMap", index, textures);
    SendMaps(shader, specularMaps, "uMaterial.specularMap", index, textures);

    shader->SetUniform("uMaterial.shininess", shininess);
}

void Material::AddNewMapIndex(unsigned int index, const aiTextureType& type) {
    if (type == aiTextureType_DIFFUSE) {
        diffuseMaps.push_back(index);
    }
    else if (type == aiTextureType_SPECULAR) {
        specularMaps.push_back(index);
    }
    else if (type == aiTextureType_HEIGHT) {
        normalMaps.push_back(index);
    }
}

void Material::SendMaps(std::shared_ptr<Shader> shader, const std::vector<unsigned int>& maps, const std::string& name, int& index,
    const std::vector<std::unique_ptr<Texture>>& textures) const {
    unsigned int textureIndex;

    for (int i = 0; i < maps.size(); i++) {
        shader->SetUniform(name + std::to_string(i + 1), index);

        if (shader->GetUniformLocation(name + std::to_string(i + 1)) != -1) {
            textureIndex = maps[i];
            textures[textureIndex]->Bind(index);
            index++;
        }
    }
}