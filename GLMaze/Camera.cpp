#include "Camera.h"

Camera::Camera() {
	io = &ImGui::GetIO();

	cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera() {
}

glm::mat4 Camera::calcTransformaton() {
	return moveCamera();
}

void Camera::handleUserInput() {
	handleKeyboardInput();
	handleMouseInput();
}

glm::mat4 Camera::moveCamera() {
	glm::mat4 transformaton(1.0f);

	float yaw = glm::radians(userInput.yaw);
	float pitch = glm::radians(userInput.pitch);

	glm::vec3 front;
	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);

	glm::vec3 cameraFront = glm::normalize(front);

	float xMove = userInput.d - userInput.a;
	float zMove = userInput.w - userInput.s;

	cameraPos += xMove * glm::normalize(glm::cross(cameraFront, cameraUp));
	cameraPos += zMove * cameraFront;

	transformaton = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp) * transformaton;

	transformaton = glm::perspective(glm::radians(60.0f), 1.5f, 0.5f, -0.5f) * transformaton;

	return transformaton;
}

void Camera::handleKeyboardInput() {
	float keyboardSensitivity = 0.01f;

	userInput.w = keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_W];
	userInput.a = keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_A];
	userInput.s = keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_S];
	userInput.d = keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_D];
}

void Camera::handleMouseInput() {
	float mouseSensitivity = 0.1f;
	float mouseXOffset = io->MousePos.x - userInput.prevMouseX;
	float mouseYOffset = userInput.prevMouseY - io->MousePos.y;

	if (io->MousePos.x != userInput.prevMouseX) {
		userInput.prevMouseX = io->MousePos.x;
	}
	if (io->MousePos.y != userInput.prevMouseY) {
		userInput.prevMouseY = io->MousePos.y;
	}

	userInput.yaw += mouseSensitivity * mouseXOffset;
	userInput.pitch += mouseSensitivity * mouseYOffset;

	if (userInput.pitch > 89.0f) {
		userInput.pitch = 89.0f;
	}
	if (userInput.pitch < -89.0f) {
		userInput.pitch = -89.0f;
	}
}
