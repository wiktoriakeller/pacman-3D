#include "Model.h"

Model::Model(const std::string& path, bool flipTextures) {
    flip = flipTextures;
    LoadModel(path);
}

void Model::Draw(std::shared_ptr<Shader> shader) {
    unsigned int materialIndex;

    for (int i = 0; i < meshes.size(); i++) {
        materialIndex = meshes[i]->GetMaterialIndex();
        
        if (materialIndex >= 0)
            materials[materialIndex]->SendMaterialToShader(shader, textures);

        meshes[i]->Draw();
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
    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<GLuint> indices(mesh->mNumFaces * 3);

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

        vertices[i].Position = glm::vec3(position->x, position->y, position->z);
        vertices[i].Normal = glm::vec3(normal->x, normal->y, normal->z);
        vertices[i].TextureCoords = glm::vec2(textureCoords->x, textureCoords->y);
        vertices[i].Tangent = glm::vec3(tangent->x, tangent->y, tangent->z);
        vertices[i].Bitangent = glm::vec3(bitangent->x, bitangent->y, bitangent->z);
    };

    aiFace face;
    int index = 0;

    for (int i = 0; i < mesh->mNumFaces; i++) {
        face = mesh->mFaces[i];

        for (int j = 0; j < face.mNumIndices; j++)
            indices[index++] = face.mIndices[j];
    }

    meshes.emplace_back(std::make_unique<Mesh>(&vertices[0], vertices.size(), &indices[0], indices.size(), mesh->mMaterialIndex));
}

void Model::LoadMaterials(const aiScene* scene) {
    aiMaterial* material;

    for (int i = 0; i < scene->mNumMaterials; i++) {
        material = scene->mMaterials[i];

        float shininess;
        material->Get(AI_MATKEY_SHININESS, shininess);

        materials.emplace_back(std::make_unique<Material>(Material(shininess)));
        LoadTextures(material, aiTextureType_DIFFUSE);
        LoadTextures(material, aiTextureType_SPECULAR);
        LoadTextures(material, aiTextureType_HEIGHT);
    }
}

void Model::LoadTextures(const aiMaterial* material, const aiTextureType& type) {
    if (material->GetTextureCount(type) > 0) {
        aiString texturePath;
        std::string path;

        for (int i = 0; i < material->GetTextureCount(type); i++) {
            material->GetTexture(type, i, &texturePath);
            path = directory + "/" + texturePath.C_Str();

            if (texturesDictionary.find(path) != texturesDictionary.end()) {
                materials[materials.size() - 1]->AddNewMapIndex(texturesDictionary[path], type);
            }
            else {
                std::cout << "New texture: " << path << "\n";
                textures.emplace_back(std::make_unique<Texture>(path, flip));
                materials[materials.size() - 1]->AddNewMapIndex(textures.size() - 1, type);
                texturesDictionary[path] = textures.size() - 1;
            }
        }
    }
}