#include "Material.h"

Material::Material(float shininess) : 
    shininess(shininess) { }

void Material::SendToShader(std::shared_ptr<Shader> shader, const std::vector<std::unique_ptr<Texture>>& textures) const {
    int index = 0;

    if (diffuseMaps.size() > 0) {
        SendMaps(shader, diffuseMaps, "uMaterial.diffuseMap", index, textures);
    }

    if (specularMaps.size() > 0) {
        shader->SetUniform("uMaterial.useNormalMap", true);
        SendMaps(shader, diffuseMaps, "uMaterial.specularMap", index, textures);
    }
    else {
        shader->SetUniform("uMaterial.useNormalMap", false);
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

void Material::SetShininess(float newShininess) {
    shininess = newShininess;
}

void Material::SendMaps(std::shared_ptr<Shader> shader, const std::vector<unsigned int>& maps, const std::string& name, int& index,
    const std::vector<std::unique_ptr<Texture>>& textures) const {

    for (int i = 0; i < maps.size(); i++) {
        shader->SetUniform(name + std::to_string(i + 1), index);

        if (shader->GetUniformLocation(name + std::to_string(i + 1)) != -1) {
            textures[maps[i]]->Bind(index);
            index++;
        }
    }
}

void Material::UnbindMaps(const std::vector<unsigned int>& maps, const std::string& name, 
    const std::vector<std::unique_ptr<Texture>>& textures) const {

    for (int i = 0; i < maps.size(); i++) {
        textures[maps[i]]->Unbind();
    }
}