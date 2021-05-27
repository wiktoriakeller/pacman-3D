#pragma once
#include <vector>
#include <GLFW/glfw3.h>

class KeyInput
{
public:
	static void SetupKeyInputs(GLFWwindow* window);
	static unsigned char GetPressedKey();
	static bool GetAnyKeyPressed();
	static void Reset();

private:
	static bool AnyKeyPressed;
	static unsigned char PressedKey;

	KeyInput();
	static void callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};