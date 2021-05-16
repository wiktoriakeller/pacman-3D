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
#include "GameObjects/Points.h"
#include "Camera.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 1024;

void init(GLFWwindow* window);

int main() {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pacman", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();
    init(window);

    std::shared_ptr<Shader> lightShader = std::make_shared<Shader>("Resources/Shaders/Vertex/vLightMaps.glsl",
        "Resources/Shaders/Fragment/fLightMaps.glsl");

    DirectionalLight dirLight(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(-0.2f, -1.0f, -0.2f));
    PointLight pointLight(glm::vec3(0.2f, 0.2f, 0.2f) * 0.2f, glm::vec3(0.7f, 0.7f, 0.7f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        1.0f, 0.09, 0.032);

    std::unique_ptr<Model> mazeModel = std::make_unique<Model>("Resources/Models/maze.obj", true);
    std::unique_ptr<Model> pacmanModel = std::make_unique<Model>("Resources/Models/pacman.obj", true);
    std::unique_ptr<Model> pointModel = std::make_unique<Model>("Resources/Models/point.obj", true);

    std::shared_ptr<Entity> player = std::make_shared<Pacman>(std::move(pacmanModel));
    std::shared_ptr<Entity> maze = std::make_shared<Entity>(std::move(mazeModel));
    std::shared_ptr<Entity> points = std::make_shared<Points>(std::move(pointModel));

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

        points->Draw(lightShader);
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
    glfwSetWindowAspectRatio(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    KeyInput::SetupKeyInputs(window);
    World::Instance().CalculatePositions();
    Renderer::Instance().SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}