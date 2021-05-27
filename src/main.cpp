#define GLEW_STATIC

#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"
#include "GameObjects/Pacman.h"
#include "GameObjects/Ghosts/Blinky.h"
#include "GameObjects/Ghosts/Pinky.h"
#include "GameObjects/Ghosts/Clyde.h"
#include "GameObjects/Ghosts/Inky.h"
#include "Camera.h"
#include "UI/UI.h"

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
        "Resources/Shaders/Fragment/fBlinnPhong.glsl");
    shaderMap["textShader"] = std::make_shared<Shader>("Resources/Shaders/Vertex/vText.glsl",
        "Resources/Shaders/Fragment/fText.glsl");
    shaderMap["spriteShader"] = std::make_shared<Shader>("Resources/Shaders/Vertex/vSprite.glsl",
        "Resources/Shaders/Fragment/fSprite.glsl");

    DirectionalLight dirLight(glm::vec3(0.15f, 0.15f, 0.15f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-2.0f, 3.0f, -1.0f));

    std::unique_ptr<Model> mazeModel = std::make_unique<Model>("Resources/Models/maze.obj", true);
    std::unique_ptr<Model> pacmanModel = std::make_unique<Model>("Resources/Models/pacman.obj", true);
    std::unique_ptr<Model> pointModel = std::make_unique<Model>("Resources/Models/point.obj", true);
    std::unique_ptr<Model> blinkyModel = std::make_unique<Model>("Resources/Models/Blinky.obj", true);
    std::unique_ptr<Model> clydeModel = std::make_unique<Model>("Resources/Models/Clyde.obj", true);
    std::unique_ptr<Model> inkyModel = std::make_unique<Model>("Resources/Models/Inky.obj", true);
    std::unique_ptr<Model> pinkyModel = std::make_unique<Model>("Resources/Models/Pinky.obj", true);

    std::shared_ptr<Entity> maze = std::make_shared<Entity>(std::move(mazeModel), false);
    std::shared_ptr<Entity> points = std::make_shared<Points>(std::move(pointModel), false);

    std::shared_ptr<Points> pointsCast = std::dynamic_pointer_cast<Points>(points);
    std::function<void(MapElement, int, int)> pointsAdder = [pointsCast](MapElement element, int x, int z) { pointsCast->AddPoints(element, x, z); };
    
    std::shared_ptr<Entity> player = std::make_shared<Pacman>(std::move(pacmanModel), pointsAdder, true);
    std::shared_ptr<Moveable> moveablePlayer = std::dynamic_pointer_cast<Moveable>(player);
    std::shared_ptr<Pacman> pacman = std::dynamic_pointer_cast<Pacman>(player);

    std::shared_ptr<Entity> clyde = std::make_shared<Clyde>(std::move(clydeModel), pacman, true);
    std::shared_ptr<Entity> blinky = std::make_shared<Blinky>(std::move(blinkyModel), pacman, true);
    std::shared_ptr<Entity> inky = std::make_shared<Inky>(std::move(inkyModel), pacman, std::dynamic_pointer_cast<Moveable>(blinky), true);
    std::shared_ptr<Entity> pinky = std::make_shared<Pinky>(std::move(pinkyModel), pacman, true);

    std::vector<std::shared_ptr<Entity>> entities = { player, blinky, clyde, inky, pinky, points, maze };

    std::vector<std::shared_ptr<Ghost>> ghosts = { std::dynamic_pointer_cast<Ghost>(blinky), std::dynamic_pointer_cast<Ghost>(clyde),
        std::dynamic_pointer_cast<Ghost>(inky), std::dynamic_pointer_cast<Ghost>(pinky) };

    bool collision;
    bool noFrightenedGhost;
    Game game;
    Camera camera(player);
    UI ui(pacman, pointsCast);
    srand(time(0));

    float deltaTime;
    float oldTimeSinceStart = (float) glfwGetTime();
    float timeSinceStart = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        timeSinceStart = (float) glfwGetTime();
        deltaTime = timeSinceStart - oldTimeSinceStart;
        oldTimeSinceStart = timeSinceStart;
      
        if (!Game::GetIsGameOver()) {
            collision = false;
            noFrightenedGhost = true;

            for (int i = 0; i < entities.size(); i++) {
                entities[i]->Update(deltaTime);
            }

            //checking collision
            for (int i = 0; i < ghosts.size(); i++) {
                if (ghosts[i]->IsFrightened()) {
                    noFrightenedGhost = false;
                }

                if (CheckCollision(moveablePlayer, ghosts[i])) {
                    if (ghosts[i]->IsFrightened()) {
                        if (!ghosts[i]->IsReturning()) {
                            pointsCast->AddPoints(MapElement::Ghost, 0, 0);
                        }
                        ghosts[i]->ReturnToHouse();
                        continue;
                    }
                    else {
                        collision = true;
                        pacman->DecreaseLives();
                        if (pacman->GetLives() < 0) {
                            pacman->RestoreLives();
                            Game::SetIsGameOver(true);
                        }
                        break;
                    }
                }
            }

            if (noFrightenedGhost) {
                pointsCast->ResetGhostScoreMultiplier();
            }

            //reseting
            if (pointsCast->GetPointsLeft() == 0 || collision) {
                for (int i = 0; i < entities.size(); i++) {
                    entities[i]->Reset();
                }

                game.Reset();
                KeyInput::Reset();
            }
        }
        else if (KeyInput::GetAnyKeyPressed()) {
            Game::SetIsGameOver(false);
        }

        //drawing
        Renderer::Instance().Clear();

        shaderMap["lightShader"]->Use();
        camera.SendToShader(shaderMap["lightShader"]);
        dirLight.SendToShader(shaderMap["lightShader"]);

        //sending light
        for (int i = 0; i < entities.size(); i++) {
            entities[i]->SendLightToShader(shaderMap["lightShader"]);
        }

        for (int i = 0; i < entities.size(); i++) {
            entities[i]->Draw(shaderMap["lightShader"]);
        }
        ui.Draw();
        
        if (Game::GetIsGameOver()) {
            ui.DrawGameBegin();
        }

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