#pragma once
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    Vertex() : Position(glm::vec3(0.0f)),
        Normal(glm::vec3(0.0f)),
        TextureCoords(glm::vec2(0.0f)),
        Tangent(glm::vec3(0.0f)),
        Bitangent(glm::vec3(0.0f)) { }

    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& textureCoords,
        const glm::vec3& tangent, const glm::vec3& bitangent) :
        Position(position),
        Normal(normal),
        TextureCoords(textureCoords),
        Tangent(tangent),
        Bitangent(bitangent) { }
};