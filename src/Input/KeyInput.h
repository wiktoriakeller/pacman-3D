#pragma once
#include <vector>
#include <GLFW/glfw3.h>

class KeyInput
{
public:
	static unsigned char PressedKey;
	static void SetupKeyInputs(GLFWwindow* window);

private:
	KeyInput();
	static void callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};