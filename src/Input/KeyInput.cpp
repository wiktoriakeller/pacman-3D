#include "KeyInput.h"

unsigned char KeyInput::PressedKey = 0;

void KeyInput::SetupKeyInputs(GLFWwindow* window) {
    glfwSetKeyCallback(window, callback);
}

void KeyInput::callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_A:
            PressedKey = 'A';
            break;
        case GLFW_KEY_D:
            PressedKey = 'D';
            break;
        case GLFW_KEY_W:
            PressedKey = 'W';
            break;
        case GLFW_KEY_S:
            PressedKey = 'S';
            break;
        default:
            break;
        }
    }
}