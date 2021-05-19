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
#include "GameObjects/Ghost.h"
#include "Camera.h"
#include "UI/Text.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

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

    std::shared_ptr<Shader> lightShader = std::make_shared<Shader>("Resources/Shaders/Vertex/vBlinnPhong.glsl", 
        "Resources/Shaders/Fragment/fBlinnPhong.glsl");
    std::shared_ptr<Shader> textShader = std::make_shared<Shader>("Resources/Shaders/Vertex/vText.glsl",
        "Resources/Shaders/Fragment/fText.glsl");

    DirectionalLight dirLight(glm::vec3(0.15f, 0.15f, 0.15f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-0.2f, -1.0f, -0.2f));
    PointLight pointLight(glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        1.0f, 0.09, 0.032);

    std::unique_ptr<Model> mazeModel = std::make_unique<Model>("Resources/Models/maze.obj", true);
    std::unique_ptr<Model> pacmanModel = std::make_unique<Model>("Resources/Models/pacman.obj", true);
    std::unique_ptr<Model> pointModel = std::make_unique<Model>("Resources/Models/point.obj", true);
    std::unique_ptr<Model> blinkyModel = std::make_unique<Model>("Resources/Models/Blinky.obj", true);
    std::unique_ptr<Model> clydeModel = std::make_unique<Model>("Resources/Models/Clyde.obj", true);
    std::unique_ptr<Model> inkyModel = std::make_unique<Model>("Resources/Models/Inky.obj", true);
    std::unique_ptr<Model> pinkyModel = std::make_unique<Model>("Resources/Models/Pinky.obj", true);

    mazeModel->ChangeMeshMaterialShininess(1, 32.0f);
    mazeModel->ChangeMeshMaterialSpecular(1, glm::vec3(0.5f, 0.5f, 0.5f));
    blinkyModel->ChangeMeshMaterialShininess(1, 128.0f);
    blinkyModel->ChangeMeshMaterialSpecular(1, glm::vec3(1.0f, 1.0f, 1.0f));

    std::shared_ptr<Entity> maze = std::make_shared<Entity>(std::move(mazeModel));
    std::shared_ptr<Entity> points = std::make_shared<Points>(std::move(pointModel));

    std::shared_ptr<Points> pointsCast = std::dynamic_pointer_cast<Points>(points);
    std::function<void(MapElement)> pointsAdder = [pointsCast](MapElement element) { pointsCast->AddPoints(element); };
    
    std::shared_ptr<Entity> player = std::make_shared<Pacman>(std::move(pacmanModel), pointsAdder);

    std::shared_ptr<Entity> blinky = std::make_shared<Ghost>(std::move(blinkyModel), 14, 11, 0, 0.0f);
    std::shared_ptr<Entity> clyde = std::make_shared<Ghost>(std::move(clydeModel), 13, 14, 10, -1.75f);
    std::shared_ptr<Entity> inky = std::make_shared<Ghost>(std::move(inkyModel), 15, 14, 20, -1.75f);
    std::shared_ptr<Entity> pinky = std::make_shared<Ghost>(std::move(pinkyModel), 17, 14, 30, -1.75f);
        
    std::vector< std::shared_ptr<Entity>> entities = { player, blinky, clyde, inky, pinky, points, maze };
    
    Camera camera(player, WINDOW_WIDTH, WINDOW_HEIGHT);
    Text text("Resources/Fonts/arial.ttf", 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), WINDOW_WIDTH, WINDOW_HEIGHT, 48.0f);

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

        lightShader->Use();
        camera.SendToShader(lightShader);
        dirLight.SendToShader(lightShader);
        pointLight.SetPosition(player->GetPosition().x, player->GetPosition().y - 0.5f, player->GetPosition().z);
        pointLight.SendToShader(lightShader);

        for (int i = 0; i < entities.size(); i++) {
            entities[i]->Draw(lightShader);
        }

        textShader->Use();
        text.Draw(textShader, "xdd", 400, 300);

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetWindowAspectRatio(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    KeyInput::SetupKeyInputs(window);
    World::Instance().CalculatePositions();
    Renderer::Instance().SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}