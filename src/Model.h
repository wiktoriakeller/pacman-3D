#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "Mesh.h"
#include "Material.h"
#include "Render/Texture.h"
#include "Render/Shader.h"

class Model
{
public:
    Model(const std::string& path, bool flipTextures = true);
    void Draw(std::shared_ptr<Shader> shader);
    void SetMeshMaterial(Material& newMaterial, unsigned int materialIndex);
    void ChangeMeshMaterialShininess(unsigned int materialIndex, float shininess);
    void ChangeMeshMaterialSpecular(unsigned int materialIndex, glm::vec3 specularColor);
    void ChangeMeshMaterialDiffuse(unsigned int materialIndex, glm::vec4 diffuseColor);
    void UseMeshMaterialSpecularColor(unsigned int materialIndex, bool use);
    void UseMeshMaterialDiffuseColor(unsigned int materialIndex, bool use);
    void SendMaterialToShader(unsigned int meshIndex, std::shared_ptr<Shader> shader);
    void UnbindMaterial(unsigned int meshIndex);
    void DrawMesh(unsigned int meshIndex);

private:
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<std::unique_ptr<Texture>> textures;
    std::vector<std::unique_ptr<Material>> materials;
    std::map<std::string, unsigned int> texturesDictionary;

    std::string directory;
    bool flip;

    void LoadModel(const std::string& path);
    void ProcessScene(const aiScene* scene);
    void LoadMesh(const aiMesh* mesh, const aiScene* scene, int index);
    void LoadMaterials(const aiScene* scene);
    void LoadTextures(const aiMaterial* material, const aiTextureType& type);
};