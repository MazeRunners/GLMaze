#pragma once
#include "imgui.h"

struct GLFWwindow;

class GUI {
public:
	struct UserInput {
		float w = 0.0f;
		float a = 0.0f;
		float s = 0.0f;
		float d = 0.0f;

		float currentMouseX = 0.0f;
		float currentMouseY = 0.0f;
		float lastMouseX = 0.0f;
		float lastMouseY = 0.0f;
		float mouseXMovement = 0.0f;
		float mouseYMovement = 0.0f;
	};

	GUI(GLFWwindow * window);
	~GUI();

	void draw();
	void render();

	void recordUserInput();
	UserInput getUserInput();

private:
	bool visible;
	ImGuiIO* io;
	struct {
		float mouseSensitivity;
		float keyboardSensitivity;
	}mConfig;
	UserInput userInput;

	void readConfig();
	void recordKeyboardInput();
	void recordMouseInput();
};
