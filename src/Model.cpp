#include "Model.h"

Model::Model(const std::string& path, bool flipTextures) {
    flip = flipTextures;
    LoadModel(path);
}

Model::Model(const std::string& path, std::vector<std::unique_ptr<Material>> meshMaterials, bool flipTextures) {
    flip = flipTextures;

    for (int i = 0; i < meshMaterials.size(); i++) {
        materials.emplace_back(std::move(meshMaterials[i]));

        if (materials[i]->GetMeshIndex() != -1) {
            meshToMaterial[materials[i]->GetMeshIndex()] = i;
        }
       
        if (materials[i]->GetAssimpMaterialIndex() != -1) {
            assimpMaterialIndexToMaterial[materials[i]->GetAssimpMaterialIndex()] = i;
        }
    }

    LoadModel(path);
}

void Model::Draw(std::shared_ptr<Shader> shader) {
    unsigned int materialIndex;

    for (int i = 0; i < meshes.size(); i++) {
        materialIndex = meshes[i]->GetMaterialIndex();
        
        if (materialIndex >= 0)
            materials[materialIndex]->SendToShader(shader, textures);

        meshes[i]->Draw();

        if (materialIndex >= 0)
            materials[materialIndex]->UnbindMaterial(textures);
    }
}

void Model::LoadModel(const std::string& path) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
        aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

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
        LoadMesh(mesh, scene, i);
    }

    LoadMaterials(scene);
}

void Model::LoadMesh(const aiMesh* mesh, const aiScene* scene, int meshIndex) {
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

    aiFace* face;
    int g = 0;

    for (int i = 0; i < mesh->mNumFaces; i++) {
        face = &(mesh->mFaces[i]);

        for (int j = 0; j < face->mNumIndices; j++)
            indices[g++] = face->mIndices[j];
    }

    unsigned int materialIndex = -1;

    if (meshToMaterial.find(meshIndex) != meshToMaterial.end()) {
        materialIndex = meshToMaterial[meshIndex];
    }
    else if (assimpMaterialIndexToMaterial.find(mesh->mMaterialIndex) != assimpMaterialIndexToMaterial.end()) {
        materialIndex = assimpMaterialIndexToMaterial[mesh->mMaterialIndex];
    }
    else {
        materialIndex = mesh->mMaterialIndex;
    }

    meshes.emplace_back(std::make_unique<Mesh>(&vertices[0], vertices.size(), &indices[0], indices.size(), materialIndex));
}

void Model::LoadMaterials(const aiScene* scene) {
    aiMaterial* material;

    for (int i = 0; i < scene->mNumMaterials; i++) {
        material = scene->mMaterials[i];

        float shininess;
        material->Get(AI_MATKEY_SHININESS, shininess);

        int matIndex = -1;

        if (assimpMaterialIndexToMaterial.find(i) != assimpMaterialIndexToMaterial.end()) {
            matIndex = assimpMaterialIndexToMaterial[i];
        }
        else {
            materials.emplace_back(std::make_unique<Material>(Material(shininess)));
            matIndex = materials.size() - 1;
        }
        
        LoadTextures(material, aiTextureType_DIFFUSE, matIndex);
        LoadTextures(material, aiTextureType_SPECULAR, matIndex);
        LoadTextures(material, aiTextureType_HEIGHT, matIndex);
    }
}

void Model::LoadTextures(const aiMaterial* material, const aiTextureType& type, int matIndex) {
    if (material->GetTextureCount(type) > 0) {
        aiString texturePath;
        std::string path;

        for (int i = 0; i < material->GetTextureCount(type); i++) {
            material->GetTexture(type, i, &texturePath);
            path = directory + "/" + texturePath.C_Str();

            if (texturesDictionary.find(path) != texturesDictionary.end()) {
                materials[matIndex]->AddNewMapIndex(texturesDictionary[path], type);
            }
            else {
                std::cout << "New texture: " << path << "\n";
                textures.emplace_back(std::make_unique<Texture>(path, flip));
                materials[matIndex]->AddNewMapIndex(textures.size() - 1, type);
                texturesDictionary[path] = textures.size() - 1;
            }
        }
    }
}

void Model::CalculateTangentAndBitangent(std::vector<unsigned int>& indices, std::vector<Vertex>& vertices) {
    for (int i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        glm::vec3 edge1 = v1.Position - v0.Position;
        glm::vec3 edge2 = v2.Position - v0.Position;

        float deltaU1 = v1.TextureCoords.x - v0.TextureCoords.y; //u1 - u0
        float deltaV1 = v1.TextureCoords.y - v0.TextureCoords.y; //v1 - v0
        float deltaU2 = v2.TextureCoords.x - v0.TextureCoords.x; //u2 - u0
        float deltaV2 = v2.TextureCoords.y - v0.TextureCoords.y; //v2 - v0

        //inverse matrix factor
        float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

        //calculating tangent vector
        v0.Tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
        v1.Tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
        v1.Tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

        //calculating bitangent vector
        v0.Bitangent.x = f * (-deltaU2 * edge1.x + deltaU1 * edge2.x);
        v1.Bitangent.y = f * (-deltaU2 * edge1.y + deltaU1 * edge2.y);
        v1.Bitangent.z = f * (-deltaU2 * edge1.z + deltaU1 * edge2.z);
    }
}