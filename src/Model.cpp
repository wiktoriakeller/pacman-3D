#include "Model.h"

Model::Model(const std::string& path, bool flipTextures) {
    flip = flipTextures;
    LoadModel(path);
}

void Model::Draw(std::shared_ptr<Shader> shader) {
    unsigned int materialIndex;

    for (int i = 0; i < meshes.size(); i++) {
        materialIndex = meshes[i].GetMaterialIndex();
        
        if (materialIndex >= 0)
            SendMaterialToShader(shader, materialIndex);

        meshes[i].Draw(shader);
    }
}

void Model::LoadModel(const std::string& path) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ASSIMP error " << importer.GetErrorString() << "\n";
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    ProcessScene(scene);
}

void Model::ProcessScene(const aiScene* scene) {
    meshes.reserve(scene->mNumMeshes);
    textures.reserve(scene->mNumTextures);
    materials.reserve(scene->mNumMaterials);

    for (int i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[i];
        LoadMesh(mesh, scene);
    }

    LoadMaterials(scene);
}

void Model::LoadMesh(const aiMesh* mesh, const aiScene* scene) {
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

void Model::LoadMaterials(const aiScene* scene) {
    aiMaterial* material;

    for (int i = 0; i < scene->mNumMaterials; i++) {
        material = scene->mMaterials[i];

        float shininess;
        material->Get(AI_MATKEY_SHININESS, shininess);

        std::vector<unsigned int> diffuseMaps(LoadTextures(material, aiTextureType_DIFFUSE));
        std::vector<unsigned int> specularMaps(LoadTextures(material, aiTextureType_SPECULAR));
        std::vector<unsigned int> normalMaps(LoadTextures(material, aiTextureType_HEIGHT));
        std::vector<unsigned int> heightMaps(LoadTextures(material, aiTextureType_AMBIENT));

        materials.emplace_back(Material(diffuseMaps, specularMaps, normalMaps, heightMaps, shininess));
    }
}

std::vector<unsigned int> Model::LoadTextures(const aiMaterial* material, const aiTextureType& type) {
    std::vector<unsigned int> maps;

    if (material->GetTextureCount(type) > 0) {
        maps.resize(material->GetTextureCount(type));
        aiString texturePath;
        std::string path;
        int index = 0;

        for (int i = 0; i < material->GetTextureCount(type); i++) {
            material->GetTexture(type, i, &texturePath);
            path = directory + "/" + texturePath.C_Str();

            if (texturesDictionary.find(path) != texturesDictionary.end()) {
                maps[index++] = texturesDictionary[path];
            }
            else {
                std::cout << "New texture: " << path << "\n";
                textures.emplace_back(std::make_shared<Texture>(path, flip));
                maps[index++] = textures.size() - 1;
                texturesDictionary[path] = textures.size() - 1;
            }
        }
    }

    return maps;
}

void Model::SendMaterialToShader(const std::shared_ptr<Shader>& shader, const unsigned int& materialIndex) const {
    int index = 0;

    SendMaps(shader, materials[materialIndex].diffuseMaps, "uMaterial.diffuseMap", index);
    SendMaps(shader, materials[materialIndex].normalMaps, "uMaterial.normalMap", index);
    SendMaps(shader, materials[materialIndex].specularMaps, "uMaterial.specularMap", index);
    SendMaps(shader, materials[materialIndex].heightMaps, "uMaterial.heightMap", index);

    shader->SetUniform("uMaterial.shininess", materials[materialIndex].shininess);
}

void Model::SendMaps(const std::shared_ptr<Shader>& shader, const std::vector<unsigned int>& maps, const std::string& name, int& index) const {
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