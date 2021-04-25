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

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoords,
        glm::vec3 tangent, glm::vec3 bitangent);
};

class Mesh 
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Material material);
    void Draw(std::shared_ptr<Shader> shader) const;

private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::shared_ptr<VertexArray> VAO;
    Material material;
};