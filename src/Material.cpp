#include "Material.h"

Material::Material(float shininess) : 
    shininess(shininess),
    diffuseColor(glm::vec3(0.0f)),
    specularColor(glm::vec3(0.0f)),
    meshIndex(-1),
    assimpMaterialIndex(-1) { }

Material::Material(glm::vec3 specularColor, float shininess) :
    shininess(shininess),
    diffuseColor(glm::vec3(0.0f)),
    specularColor(specularColor),
    meshIndex(-1),
    assimpMaterialIndex(-1) { }

Material::Material(glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess) :
    shininess(shininess),
    diffuseColor(diffuseColor),
    specularColor(specularColor),
    meshIndex(-1),
    assimpMaterialIndex(-1) { }

void Material::SendToShader(std::shared_ptr<Shader> shader, const std::vector<std::unique_ptr<Texture>>& textures) const {
    int index = 0;

    if (diffuseMaps.size() > 0) {
        shader->SetUniform("uMaterial.useDiffuseMap", true);
        SendMaps(shader, diffuseMaps, "uMaterial.diffuseMap", index, textures);
    }
    else {
        shader->SetUniform("uMaterial.useDiffuseMap", false);
        shader->SetUniform("uMaterial.diffuseColor", diffuseColor);
    }

    if (specularMaps.size() > 0) {
        shader->SetUniform("uMaterial.useSpecMap", true);
        SendMaps(shader, diffuseMaps, "uMaterial.specularMap", index, textures);
    }
    else {
        shader->SetUniform("uMaterial.useSpecMap", false);
        shader->SetUniform("uMaterial.specularColor", specularColor);
    }

    if (normalMaps.size() > 0) {
        shader->SetUniform("uMaterial.useNormalMap", true);
        SendMaps(shader, normalMaps, "uMaterial.normalMap", index, textures);
    }
    else {
        shader->SetUniform("uMaterial.useNormalMap", false);
    }

    shader->SetUniform("uMaterial.shininess", shininess);
}

void Material::UnbindMaterial(const std::vector<std::unique_ptr<Texture>>& textures) const {
    if (diffuseMaps.size() > 0) {
        UnbindMaps(diffuseMaps, "uMaterial.diffuseMap", textures);
    }

    if (specularMaps.size() > 0) {
        UnbindMaps(diffuseMaps, "uMaterial.specularMap", textures);
    }

    if (normalMaps.size() > 0) {
        UnbindMaps(normalMaps, "uMaterial.normalMap", textures);
    }
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

void Material::SetDiffuseColor(glm::vec3 color) {
    diffuseColor = color;
}

void Material::SetSpecularColor(glm::vec3 color) {
    specularColor = color;
}

void Material::SetShininess(float newShininess) {
    shininess = newShininess;
}

void Material::SetMeshIndex(int index) {
    meshIndex = index;
}

int Material::GetMeshIndex() const {
    return meshIndex;
}

void Material::SetAssimpMaterialIndex(int index) {
    assimpMaterialIndex = index;
}

int Material::GetAssimpMaterialIndex() const {
    return assimpMaterialIndex;
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

void Material::UnbindMaps(const std::vector<unsigned int>& maps, const std::string& name, 
    const std::vector<std::unique_ptr<Texture>>& textures) const {
    unsigned int textureIndex;

    for (int i = 0; i < maps.size(); i++) {
        textureIndex = maps[i];
        textures[textureIndex]->Unbind();
    }
}