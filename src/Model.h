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
        for (int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<Mesh> meshes;
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
        textures.reserve(scene->mNumMaterials);

        for(int i = 0; i < scene->mNumMeshes; i++) {
            const aiMesh* mesh = scene->mMeshes[i];
            CreateMeshAndMaterial(mesh, scene);
        }
    }

    void CreateMeshAndMaterial(const aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

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

            Vertex vertex(glm::vec3(position->x, position->y, position->z),
                glm::vec3(normal->x, normal->y, normal->z),
                glm::vec2(textureCoords->x, textureCoords->y),
                glm::vec3(tangent->x, tangent->y, tangent->z),
                glm::vec3(bitangent->x, bitangent->y, bitangent->z));

            vertices.push_back(vertex);

        };

        aiFace face;
        for (int i = 0; i < mesh->mNumFaces; i++) {
            face = mesh->mFaces[i];

            for (int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        float shininess;
        material->Get(AI_MATKEY_SHININESS, shininess);

        //std::cout << material->GetTextureCount(aiTextureType_DIFFUSE) << "\n";

        std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
        std::vector<std::shared_ptr<Texture>> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
        std::vector<std::shared_ptr<Texture>> normalMaps = LoadTextures(material, aiTextureType_NORMALS);
        std::vector<std::shared_ptr<Texture>> heightMaps = LoadTextures(material, aiTextureType_HEIGHT);

        meshes.push_back(Mesh(vertices, indices, Material(diffuseMaps, specularMaps, normalMaps, heightMaps, shininess)));
    }

    std::vector<std::shared_ptr<Texture>> LoadTextures(aiMaterial* material, aiTextureType type) {
        std::vector<std::shared_ptr<Texture>> maps;
        for (int i = 0; i < material->GetTextureCount(type); i++) {
            aiString texturePath;
            material->GetTexture(type, i, &texturePath);

            std::string path = directory + "/" + texturePath.C_Str();

            //texture has been already loaded
            if (texturesDictionary.find(path) != texturesDictionary.end()) {
                maps.push_back(textures[texturesDictionary[path]]);
            }
            else {
                std::cout << "new texture "<< path << "\n";
                textures.emplace_back(std::make_shared<Texture>(path, flip));
                maps.push_back(textures[textures.size() - 1]);
                texturesDictionary[path] = textures.size() - 1;
            }
        }

        return maps;
    }
};