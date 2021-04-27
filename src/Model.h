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

private:
    std::vector<Mesh> meshes;
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<Material> materials;
    std::map<std::string, unsigned int> texturesDictionary;

    std::string directory;
    bool flip;

    void LoadModel(const std::string& path);
    void ProcessScene(const aiScene* scene);
    void LoadMesh(const aiMesh* mesh, const aiScene* scene);
    void LoadMaterials(const aiScene* scene);
    std::vector<unsigned int> LoadTextures(const aiMaterial* material, const aiTextureType& type);
    void SendMaterialToShader(const std::shared_ptr<Shader>& shader, const unsigned int& materialIndex) const;
    void SendMaps(const std::shared_ptr<Shader>& shader, const std::vector<unsigned int>& maps, const std::string& name, int& index) const;
};