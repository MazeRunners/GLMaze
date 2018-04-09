#pragma once

#include "imgui.h"
#include "ImGUIManager.h"
#include <glm/gtc/matrix_transform.hpp>

struct UserInput {
	float w = 0.0f;
	float a = 0.0f;
	float s = 0.0f;
	float d = 0.0f;

	float prevMouseX = 0.0f;
	float prevMouseY = 0.0f;
	float yaw = -90.0f;
	float pitch = 0.0f;
};

class Camera {
public:
	Camera();
	~Camera();
	glm::mat4 calcTransformaton();
	void handleUserInput();

private:
	glm::mat4 moveCamera();
	void handleKeyboardInput();
	void handleMouseInput();

	ImGuiIO* io;
	UserInput userInput;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
};

