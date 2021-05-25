#define GLEW_STATIC

#include "Model.h"
#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"
#include "GameObjects/Pacman.h"
#include "GameObjects/Points.h"
#include "GameObjects/Ghosts/Blinky.h"
#include "GameObjects/Ghosts/Pinky.h"
#include "GameObjects/Ghosts/Clyde.h"
#include "GameObjects/Ghosts/Inky.h"
#include "Camera.h"
#include "UI/UI.h"
#include "Game.h"

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

    window = glfwCreateWindow(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, "Pacman", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();
    init(window);

    shaderMap["lightShader"] = std::make_shared<Shader>("Resources/Shaders/Vertex/vBlinnPhong.glsl",
        "Resources/Shaders/Fragment/fBlinnPhong.glsl");;
    shaderMap["textShader"] = std::make_shared<Shader>("Resources/Shaders/Vertex/vText.glsl",
        "Resources/Shaders/Fragment/fText.glsl");
    shaderMap["spriteShader"] = std::make_shared<Shader>("Resources/Shaders/Vertex/vSprite.glsl",
        "Resources/Shaders/Fragment/fSprite.glsl");
    shaderMap["screenShader"] = std::make_shared<Shader>("Resources/Shaders/Vertex/vScreenShader.glsl",
        "Resources/Shaders/Fragment/fScreenShader.glsl");

    DirectionalLight dirLight(glm::vec3(0.15f, 0.15f, 0.15f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-2.0f, 3.0f, -1.0f));
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

    std::shared_ptr<Entity> blinky = std::make_shared<Blinky>(std::move(blinkyModel), std::dynamic_pointer_cast<Moveable>(player));
    std::shared_ptr<Entity> clyde = std::make_shared<Clyde>(std::move(clydeModel), std::dynamic_pointer_cast<Moveable>(player));
    std::shared_ptr<Entity> inky = std::make_shared<Inky>(std::move(inkyModel), std::dynamic_pointer_cast<Moveable>(player), 
        std::dynamic_pointer_cast<Moveable>(blinky));
    std::shared_ptr<Entity> pinky = std::make_shared<Pinky>(std::move(pinkyModel), std::dynamic_pointer_cast<Moveable>(player));

    std::vector< std::shared_ptr<Entity>> entities = { player, blinky, clyde, inky, pinky, points, maze };

    Camera camera(player);
    UI ui;

    float deltaTime;
    float oldTimeSinceStart = glfwGetTime();
    float timeSinceStart = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        timeSinceStart = (float) glfwGetTime();
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;
       
        for (int i = 0; i < entities.size(); i++) {
            entities[i]->Update(deltaTime);
        }

        //drawing
        Renderer::Instance().Clear();

        shaderMap["lightShader"]->Use();
        camera.SendToShader(shaderMap["lightShader"]);
        dirLight.SendToShader(shaderMap["lightShader"]);
        pointLight.SetPosition(player->GetPosition().x, player->GetPosition().y - 0.5f, player->GetPosition().z);
        pointLight.SendToShader(shaderMap["lightShader"]);

        for (int i = 0; i < entities.size(); i++) {
            entities[i]->Draw(shaderMap["lightShader"]);
        }

        ui.Draw();

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
    glfwSetWindowAspectRatio(window, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
    KeyInput::SetupKeyInputs(window);
    World::Instance().CalculatePositions();
    Renderer::Instance().SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}