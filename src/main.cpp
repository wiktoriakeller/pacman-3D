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
#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"

void init(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_SAMPLES, 4);
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

    Renderer::Instance().SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 0.1f));

    std::shared_ptr<Shader> lightSourceShader = std::make_shared<Shader>("Resources/Shaders/Vertex/vLightSource.glsl",
        "Resources/Shaders/Fragment/fLightSource.glsl");

    std::shared_ptr<Shader> lightShader = std::make_shared<Shader>("Resources/Shaders/Vertex/vLightMaps.glsl",
        "Resources/Shaders/Fragment/fLightMaps.glsl");

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
    
    BufferLayout layout = {
        {AttributeDataType::Float3, 3},
        {AttributeDataType::Float3, 3},
        {AttributeDataType::Float2, 2}
    };

    //light source
    std::unique_ptr<VertexBuffer> lightVBO = std::make_unique<VertexBuffer>(vertices, verticesCount * sizeof(GLfloat), GL_STATIC_DRAW, layout);
    std::unique_ptr<IndexBuffer> lightIBO = std::make_unique<IndexBuffer>(indices, indicesCount, GL_STATIC_DRAW);

    std::unique_ptr<VertexArray> lightVAO = std::make_unique<VertexArray>();
    lightVAO->AddVertexBuffer(std::move(lightVBO));
    lightVAO->SetIndexBuffer(std::move(lightIBO));

    DirectionalLight dirLight(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(-0.2f, -1.0f, -0.2f));

    PointLight pointLight(glm::vec3(0.2f, 0.2f, 0.2f) * 0.1f, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-2.5f, 0.0f, -1.0f),
        1.0f, 0.09, 0.032);

    float deltaTime;
    float oldTimeSinceStart = glfwGetTime();
    float timeSinceStart = 0.0f;

    Model backpack("Resources/Models/backpack/backpack.obj", false);

    while (!glfwWindowShouldClose(window)) {
        timeSinceStart = (float) glfwGetTime();
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;

        Renderer::Instance().Clear();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, -3.5f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        //light
        glm::mat4 lightSourceModel = glm::mat4(1.0f);
        glm::vec3 position = glm::vec3(cos(glfwGetTime()) * 2.0f, 0.0f, sin(glfwGetTime()) * 2.0f);
        lightSourceModel = glm::translate(lightSourceModel, position);
        lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.4f));
        pointLight.SetPosition(position);

        lightSourceShader->Use();
        lightSourceShader->SetUniform("uLightColor", pointLight.GetDiffuse());
        lightSourceShader->SetUniform("uView", view);
        lightSourceShader->SetUniform("uProjection", projection);
        lightSourceShader->SetUniform("uModel", lightSourceModel);
        
        lightVAO->Bind();
        Renderer::Instance().Draw(lightVAO->GetIBOCount());

        //object
        glm::mat4 backpackModel = glm::mat4(1.0f);
        backpackModel = glm::rotate(backpackModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(backpackModel)));
        
        lightShader->Use();
        dirLight.SendToShader(lightShader);
        pointLight.SendToShader(lightShader);

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
    glEnable(GL_MULTISAMPLE);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}