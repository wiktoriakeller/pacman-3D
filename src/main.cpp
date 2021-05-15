#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "Model.h"
#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"
#include "GameObjects/Pacman.h"
#include "Camera.h"

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
    glewInit();
    init(window);

    Renderer::Instance().SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 0.1f));

    std::shared_ptr<Shader> lightShader = std::make_shared<Shader>("Resources/Shaders/Vertex/vLightMaps.glsl",
        "Resources/Shaders/Fragment/fLightMaps.glsl");

    DirectionalLight dirLight(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(-0.2f, -1.0f, -0.2f));

    PointLight pointLight(glm::vec3(0.2f, 0.2f, 0.2f) * 0.2f, glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        1.0f, 0.09, 0.032);

    std::shared_ptr<Model> mazeModel = std::make_shared<Model>("Resources/maze.obj", true);
    std::shared_ptr<Model> pacmanModel = std::make_shared<Model>("Resources/pacman.obj", true);

    std::shared_ptr<Entity> player = std::make_shared<Pacman>(pacmanModel);
    std::shared_ptr<Entity> point = std::make_shared<Entity>(pacmanModel);
    point->Scale(glm::vec3(0.3, 0.3, 0.3));
    std::shared_ptr<Entity> maze = std::make_shared<Entity>(mazeModel);

    Camera camera(player);
    lightShader->Use();
    dirLight.SendToShader(lightShader);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    lightShader->SetUniform("uProjection", projection);

    float deltaTime;
    float oldTimeSinceStart = glfwGetTime();
    float timeSinceStart = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        timeSinceStart = (float) glfwGetTime();
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;
       
        player->Update(deltaTime);

        //drawing
        Renderer::Instance().Clear();
        camera.SendToShader(lightShader);

        pointLight.SetPosition(glm::vec3(cos(glfwGetTime()) * 2.0f + player->GetPosition().x, 
            player->GetPosition().y, sin(glfwGetTime()) * 2.0f + player->GetPosition().z));
        pointLight.SendToShader(lightShader);

        for (int y = 0; y < World::HEIGHT; y++) {
            for (int x = 0; x < World::WIDTH; x++) {
                if (World::Instance().GetMapElement(x, y) == MapElement::Point || World::Instance().GetMapElement(x, y) == MapElement::Power) {
                    point->SetPosition(World::Instance().GetPosition(x, y));
                    point->Draw(lightShader);
                }
            }
        }

        player->Draw(lightShader);
        maze->Draw(lightShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void init(GLFWwindow* window) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glfwSetWindowAspectRatio(window, 8, 6);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    KeyInput::SetupKeyInputs(window);
    World::Instance().CalculatePositions();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}