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
#include "Camera.h"

void init(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods);

glm::vec3 move;

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

    DirectionalLight dirLight(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(-0.2f, -1.0f, -0.2f));

    PointLight pointLight(glm::vec3(0.2f, 0.2f, 0.2f) * 0.2f, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-2.5f, 0.0f, -1.0f),
        1.0f, 0.09, 0.032);

    float deltaTime;
    float oldTimeSinceStart = glfwGetTime();
    float timeSinceStart = 0.0f;

    Model mazeModel("Resources/maze.obj", true);
    glm::mat4 mazeModelMatrix = glm::mat4(1.0f);
    Model playerModel("Resources/pacman.obj", true);
    glm::mat4 playerModelMatrix = glm::mat4(1.0f);

    Camera camera;

    while (!glfwWindowShouldClose(window)) {
        timeSinceStart = (float) glfwGetTime();
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;

        Renderer::Instance().Clear();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        camera.LookAt(playerModelMatrix);
        glm::mat4 view = camera.GetView();

        //object
        playerModelMatrix = glm::translate(playerModelMatrix, move * deltaTime);
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(playerModelMatrix)));
        
        pointLight.SetPosition(playerModelMatrix[3]);
        lightShader->Use();
        dirLight.SendToShader(lightShader);
        pointLight.SendToShader(lightShader);

        lightShader->SetUniform("uView", view);
        lightShader->SetUniform("uProjection", projection);
        lightShader->SetUniform("uModel", playerModelMatrix);
        lightShader->SetUniform("uNormalMatrix", normalMatrix);
        lightShader->SetUniform("uViewPosition", camera.GetPosition());

        playerModel.Draw(lightShader);

        normalMatrix = glm::transpose(glm::inverse(glm::mat3(mazeModelMatrix)));
        lightShader->SetUniform("uModel", mazeModelMatrix);
        lightShader->SetUniform("uNormalMatrix", normalMatrix);

        mazeModel.Draw(lightShader);


        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwSetKeyCallback(window, keyboardInput);
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

void keyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float speed = 4;


        move = glm::vec3(0, 0, 0);
    if (action == GLFW_PRESS or action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_A:
            move.x = 1 * speed;
            break;
        case GLFW_KEY_D:
            move.x = -1 * speed;
            break;
        case GLFW_KEY_W:
            move.z = 1 * speed;
            break;
        case GLFW_KEY_S:
            move.z = -1 * speed;
            break;
        default:
            break;
        }
    }
}
