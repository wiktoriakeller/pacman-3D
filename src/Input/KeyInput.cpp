#include "KeyInput.h"

unsigned char KeyInput::PressedKey = 0;
bool KeyInput::AnyKeyPressed = false;

void KeyInput::SetupKeyInputs(GLFWwindow* window) {
    glfwSetKeyCallback(window, callback);
}

unsigned char KeyInput::GetPressedKey() {
    return PressedKey;
}

bool KeyInput::GetAnyKeyPressed() {
    return AnyKeyPressed;
}

void KeyInput::Reset() {
    AnyKeyPressed = false;
    PressedKey = 0;
}

void KeyInput::callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        AnyKeyPressed = true;

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