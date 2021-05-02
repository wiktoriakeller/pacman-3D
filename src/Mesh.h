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
    Mesh(Vertex* vertex, unsigned int verticesSize, GLuint* index, unsigned int indicesSize, unsigned int materialIndex);
    void Draw() const;
    unsigned int GetMaterialIndex() const;
    void SetMaterialIndex(unsigned int newMaterialIndex);

private:
    std::unique_ptr<VertexArray> VAO;
    unsigned int materialIndex;
};