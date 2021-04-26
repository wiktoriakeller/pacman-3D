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
#include "Vertex.h"

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