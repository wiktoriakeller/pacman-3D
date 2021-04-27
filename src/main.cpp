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
    std::shared_ptr<VertexBuffer> lightVBO = std::make_shared<VertexBuffer>(vertices, verticesCount * sizeof(GLfloat), GL_STATIC_DRAW, layout);
    std::shared_ptr<IndexBuffer> lightIBO = std::make_shared<IndexBuffer>(indices, indicesCount, GL_STATIC_DRAW);

    std::shared_ptr<VertexArray> lightVAO = std::make_shared<VertexArray>();
    lightVAO->AddVertexBuffer(lightVBO);
    lightVAO->SetIndexBuffer(lightIBO);

    DirectionalLight dirLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.2f, -1.0f, -0.2f));

    PointLight pointLight(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec3(0.8f, 0.8f, 0.8f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-2.5f, 0.0f, -1.0f),
        1.0f, 0.14, 0.07);

    float deltaTime;
    float oldTimeSinceStart = glfwGetTime();
    float timeSinceStart = 0.0f;

    Model backpack("Resources/Models/backpack/backpack.obj", false);
    glm::vec3 pointLightPos = glm::vec3(0.0f);
    float translationX = 0.3f;

    while (!glfwWindowShouldClose(window)) {
        timeSinceStart = (float) glfwGetTime();
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;

        Renderer::Instance().Clear();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, -5.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        //light
        glm::mat4 lightSourceModel = glm::mat4(1.0f);

        if (pointLightPos.x >= 2.5f) {
            pointLightPos.x = 2.5f;
            translationX = -0.3f;
        }
        else if (pointLightPos.x <= -2.5f) {
            pointLightPos.x = -2.5f;
            translationX = 0.3f;
        }

        pointLightPos.x += translationX * deltaTime;

        lightSourceModel = glm::translate(lightSourceModel, pointLightPos);
        lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.4f));
        pointLight.SetPosition(pointLightPos);

        lightSourceShader->Use();
        lightSourceShader->SetUniform("uLightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        lightSourceShader->SetUniform("uView", view);
        lightSourceShader->SetUniform("uProjection", projection);
        lightSourceShader->SetUniform("uModel", lightSourceModel);
        
        Renderer::Instance().Draw(lightVAO);

        //object
        glm::mat4 backpackModel = glm::mat4(1.0f);
        backpackModel = glm::translate(backpackModel, glm::vec3(2.5f, 0.0f, 0.0f));
        backpackModel = glm::rotate(backpackModel, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}