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
    Model(const std::string& path, bool flipTexture = true)  {
        flip = flipTexture;
        LoadModel(path);
    }

    void Draw(std::shared_ptr<Shader> shader) {
        for (int i = 0; i < meshes.size(); i++) {
            SendMaterialToShader(shader, meshes[i].GetMaterialIndex());
            meshes[i].Draw(shader);
        }
    }

private:
    std::vector<Mesh> meshes;
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<Material> materials;
    std::map<std::string, unsigned int> texturesDictionary;

    std::string directory;
    bool flip;

    void LoadModel(const std::string& path) {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
        
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ASSIMP error " << importer.GetErrorString() << "\n";
            return;
        }
        
        directory = path.substr(0, path.find_last_of('/'));
        ProcessScene(scene);
    }

    void ProcessScene(const aiScene* scene) {
        meshes.reserve(scene->mNumMeshes);
        textures.reserve(scene->mNumTextures);
        materials.reserve(scene->mNumMaterials);

        for(int i = 0; i < scene->mNumMeshes; i++) {
            const aiMesh* mesh = scene->mMeshes[i];
            LoadMesh(mesh, scene);
        }

        LoadMaterials(scene);
    }

    void LoadMesh(const aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices(mesh->mNumFaces * 3);

        vertices.reserve(mesh->mNumVertices);
        const aiVector3D zero(0.0f, 0.0f, 0.0f);

        for (int i = 0; i < mesh->mNumVertices; i++) {
            const aiVector3D* position = &(mesh->mVertices[i]);
            const aiVector3D* normal = &(mesh->mNormals[i]);
            const aiVector3D* textureCoords = mesh->HasTextureCoords(0) ?
                &(mesh->mTextureCoords[0][i]) : &zero;
            const aiVector3D* tangent = mesh->HasTangentsAndBitangents() ?
                &(mesh->mTangents[i]) : &zero;
            const aiVector3D* bitangent = mesh->HasTangentsAndBitangents() ?
                &(mesh->mBitangents[i]) : &zero;

            vertices.emplace_back(Vertex(glm::vec3(position->x, position->y, position->z),
                glm::vec3(normal->x, normal->y, normal->z),
                glm::vec2(textureCoords->x, textureCoords->y),
                glm::vec3(tangent->x, tangent->y, tangent->z),
                glm::vec3(bitangent->x, bitangent->y, bitangent->z)));
        };

        aiFace face;
        int index = 0;

        for (int i = 0; i < mesh->mNumFaces; i++) {
            face = mesh->mFaces[i];

            for (int j = 0; j < face.mNumIndices; j++)
                indices[index++] = face.mIndices[j];
        }

        meshes.emplace_back(Mesh(&vertices[0], vertices.size(), &indices[0], indices.size(), mesh->mMaterialIndex));
    }

    void LoadMaterials(const aiScene* scene) {
        aiMaterial* material;
        
        for (int i = 0; i < scene->mNumMaterials; i++) {
            material = scene->mMaterials[i];

            float shininess;
            material->Get(AI_MATKEY_SHININESS, shininess);

            const std::vector<unsigned int> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
            const std::vector<unsigned int> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
            const std::vector<unsigned int> normalMaps = LoadTextures(material, aiTextureType_NORMALS);
            const std::vector<unsigned int> heightMaps = LoadTextures(material, aiTextureType_HEIGHT);

            materials.emplace_back(Material(diffuseMaps, specularMaps, normalMaps, heightMaps, shininess));
        }
    }

    std::vector<unsigned int>& LoadTextures(const aiMaterial* material, aiTextureType type) {
        std::vector<unsigned int> maps(material->GetTextureCount(type));

        aiString texturePath;
        std::string path;   
        int index = 0;

        for (int i = 0; i < material->GetTextureCount(type); i++) {
            material->GetTexture(type, i, &texturePath);
            path = directory + "/" + texturePath.C_Str();

            //texture has been already loaded
            if (texturesDictionary.find(path) != texturesDictionary.end()) {
                maps[index++] = texturesDictionary[path];
            }
            else {
                std::cout << "new texture: " << path << "\n";
                textures.emplace_back(std::make_shared<Texture>(path, flip));
                maps[index++] = textures.size() - 1;
                texturesDictionary[path] = textures.size() - 1;
            }
        }

        return maps;
    }

    void SendMaterialToShader(const std::shared_ptr<Shader>& shader, const unsigned int& materialIndex) const {
        int index = 0;

        SendMaps(shader, materials[materialIndex].diffuseMaps, "uMaterial.diffuseMap", index);
        SendMaps(shader, materials[materialIndex].specularMaps, "uMaterial.normalMap", index);
        SendMaps(shader, materials[materialIndex].normalMaps, "uMaterial.specularMap", index);
        SendMaps(shader, materials[materialIndex].heightMaps, "uMaterial.heightMap", index);

        shader->SetUniform("uMaterial.shininess", materials[materialIndex].shininess);
    }

    void SendMaps(const std::shared_ptr<Shader>& shader, const std::vector<unsigned int>& maps, const std::string& name, int& index) const {
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
};