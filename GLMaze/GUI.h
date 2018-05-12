#pragma once
#include "imgui.h"

#include "GameConfig.h"

struct GLFWwindow;

class GUI {
public:
	struct UserInput {
		float w = 0.0f;
		float a = 0.0f;
		float s = 0.0f;
		float d = 0.0f;

		float up = 0.0f;
		float down = 0.0f;
		float left = 0.0f;
		float right = 0.0f;
		float front = 0.0f;
		float back = 0.0f;

		float currentMouseX = 0.0f;
		float currentMouseY = 0.0f;

		float lastMouseX = 0.0f;
		float lastMouseY = 0.0f;

		float mouseXMovement = 0.0f;
		float mouseYMovement = 0.0f;
	};

	GUI(GLFWwindow * window, GameConfig::Parameters config);
	~GUI();

	void draw();
	void render();

	void recordUserInput();
	UserInput getUserInput();

private:
	bool visible;

	ImGuiIO* io;
	float mouseSensitivity;
	float keyboardSensitivity;

	UserInput userInput;

	void recordKeyboardInput();
	void recordMouseInput();
};
