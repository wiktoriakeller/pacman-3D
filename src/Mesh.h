#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Render/IndexBuffer.h"
#include "Render/VertexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/BufferLayout.h"
#include "Render/Renderer.h"
#include "Render/Shader.h"
#include "Material.h"

struct Vertex 
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textureCoords,
        const glm::vec3& tangent, const glm::vec3& bitangent);
};

class Mesh 
{
public:
    Mesh(Vertex* vertex, const unsigned int& verticesSize, GLuint* index, const unsigned int& indicesSize, unsigned int materialIndex);
    void Draw(std::shared_ptr<Shader> shader) const;
    unsigned int GetMaterialIndex() const;

private:
    std::shared_ptr<VertexArray> VAO;
    unsigned int materialIndex;
};