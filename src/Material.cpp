#include "Material.h"

Material::Material(float shininess) : 
    shininess(shininess),
    diffuseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
    specularColor(glm::vec3(0.0f, 0.0f, 0.0f)) {
    useDiffuseColor = false;
    useSpecularColor = false;
}

Material::Material(glm::vec3 specularCol, float shininess) :
    diffuseColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
    specularColor(specularCol),
    shininess(shininess) {
    useDiffuseColor = false;
    useSpecularColor = true;
}

Material::Material(glm::vec4 diffuseCol, glm::vec3 specularCol, float shininess) :
    diffuseColor(diffuseCol),
    specularColor(specularCol),
    shininess(shininess) {
    useDiffuseColor = true;
    useSpecularColor = true;
}

void Material::SendToShader(std::shared_ptr<Shader> shader, const std::vector<std::unique_ptr<Texture>>& textures) const {
    int index = 0;

    shader->SetUniform("uMaterial.useDiffuseColor", useDiffuseColor);
    if (diffuseMaps.size() > 0 && !useDiffuseColor) {
        SendMaps(shader, diffuseMaps, "uMaterial.diffuseMap", index, textures);
    }
    else if (useDiffuseColor) {
        shader->SetUniform("uMaterial.diffuseColor", diffuseColor);
    }

    shader->SetUniform("uMaterial.useSpecularColor", useSpecularColor);
    if (specularMaps.size() > 0 && !useSpecularColor) {
        SendMaps(shader, specularMaps, "uMaterial.specularMap", index, textures);
    }
    else if (useSpecularColor) {
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
    if (diffuseMaps.size() > 0 && !useDiffuseColor) {
        UnbindMaps(diffuseMaps, "uMaterial.diffuseMap", textures);
    }

    if (specularMaps.size() > 0 && !useSpecularColor) {
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

void Material::SetDiffuseColor(glm::vec4 color) {
    diffuseColor = color;
}

void Material::SetSpecularColor(glm::vec3 color) {
    specularColor = color;
}

void Material::UseDiffuseColor(bool use) {
    useDiffuseColor = use;
}

void Material::UseSpecularColor(bool use) {
    useSpecularColor = use;
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