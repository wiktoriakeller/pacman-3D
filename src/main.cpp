#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>

#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/BufferLayout.h"
#include "Render/VertexArray.h"
#include "Render/Renderer.h"
#include "Render/Texture.h"
#include "Material.h"
#include "Model.h"

void init(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Pacman", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    init(window);
    glewInit();

    Renderer::Instance().SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 0.2f));

    std::shared_ptr<Shader> lightSourceShader = std::make_shared<Shader>("Resources/Shaders/Vertex/vLightSource.glsl",
        "Resources/Shaders/Fragment/fLightSource.glsl");

    std::shared_ptr<Shader> lightShader = std::make_shared<Shader>("Resources/Shaders/Vertex/vLightMaps.glsl",
        "Resources/Shaders/Fragment/fLightMaps.glsl");

    /*
    std::shared_ptr<Texture> boxTexture = std::make_shared<Texture>("Resources/Textures/box.png");
    std::shared_ptr<Texture> boxSpecular = std::make_shared<Texture>("Resources/Textures/box_specular.png");

    Material boxMaterial(boxTexture, boxSpecular, 64.0f);
    */

    GLfloat vertices[] = {
        //position          //normals           //texture
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int verticesCount = 6 * 4 * 8;

    GLuint indices[] = {
         0, 1, 3,
         3, 1, 2,

         4, 5, 7,
         7, 5, 6,

         8, 9, 11,
         11, 9, 10,

         12, 13, 15,
         15, 13, 14,

         16, 17, 19,
         19, 17, 18,

         20, 21, 23,
         23, 21, 22
    };

    unsigned int indicesCount = 3 * 2 * 6;

    //std::shared_ptr<VertexArray> cubeVAO = std::make_shared<VertexArray>();
    
    BufferLayout layout = {
        {AttributeDataType::Float3, 3},
        {AttributeDataType::Float3, 3},
        {AttributeDataType::Float2, 2}
    };

    /*
    std::shared_ptr<VertexBuffer> cubeVBO = std::make_shared<VertexBuffer>(vertices, verticesCount * sizeof(GLfloat), GL_STATIC_DRAW, layout);
    cubeVAO->AddVertexBuffer(cubeVBO);

    std::shared_ptr<IndexBuffer> cubeIBO = std::make_shared<IndexBuffer>(indices, indicesCount, GL_STATIC_DRAW);
    cubeVAO->SetIndexBuffer(cubeIBO);

    */

    //light
    std::shared_ptr<VertexArray> lightVAO = std::make_shared<VertexArray>();

    std::shared_ptr<VertexBuffer> lightVBO = std::make_shared<VertexBuffer>(vertices, verticesCount * sizeof(GLfloat), GL_STATIC_DRAW, layout);
    lightVAO->AddVertexBuffer(lightVBO);

    std::shared_ptr<IndexBuffer> lightIBO = std::make_shared<IndexBuffer>(indices, indicesCount, GL_STATIC_DRAW);
    lightVAO->SetIndexBuffer(lightIBO);

    float deltaTime;
    float oldTimeSinceStart = glfwGetTime();
    float timeSinceStart = 0.0f;

    Model backpack("Resources/Models/backpack/backpack.obj", false);
    glm::mat4 backpackModel = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(window)) {
        timeSinceStart = (float) glfwGetTime();
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;

        Renderer::Instance().Clear();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(50.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, -5.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        //light
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::mat4 lightSourceModel = glm::mat4(1.0f);
        glm::vec3 lightSourcePosition = glm::vec3(cos(glfwGetTime()) * 2.0f, 0.0f, sin(glfwGetTime()) * 2.0f);

        lightSourceModel = glm::translate(lightSourceModel, lightSourcePosition);
        lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.3f));

        lightSourceShader->Use();
        lightSourceShader->SetUniform("uLightColor", lightColor);
        lightSourceShader->SetUniform("uView", view);
        lightSourceShader->SetUniform("uProjection", projection);
        lightSourceShader->SetUniform("uModel", lightSourceModel);

        Renderer::Instance().Draw(lightVAO);

        //cube
        lightShader->Use();

        //boxMaterial.SendMaterialToShader(lightShader, 0, 1);

        lightShader->SetUniform("uLight.position", lightSourcePosition);
        lightShader->SetUniform("uLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightShader->SetUniform("uLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        lightShader->SetUniform("uLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        backpackModel = glm::rotate(backpackModel, glm::radians(deltaTime * 30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(backpackModel)));

        lightShader->SetUniform("uView", view);
        lightShader->SetUniform("uProjection", projection);
        lightShader->SetUniform("uModel", backpackModel);
        lightShader->SetUniform("uNormalMatrix", normalMatrix);
        lightShader->SetUniform("uViewPosition", glm::vec3(0.0f, 0.0f, -5.0f));

        backpack.Draw(lightShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void init(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}