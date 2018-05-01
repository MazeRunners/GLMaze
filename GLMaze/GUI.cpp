#include "GUI.h"
#include "imgui_impl_glfw_gl3.h"

#include <GLFW/glfw3.h>

GUI::GUI(GLFWwindow * window) {
	visible = true;

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);

	io = &ImGui::GetIO();
}

GUI::~GUI() {
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void GUI::toNextFrame() {
	ImGui_ImplGlfwGL3_NewFrame();
}

void GUI::render() {
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::recordUserInput() {
	recordMouseInput();
	recordKeyboardInput();
}

GUI::UserInput GUI::getUserInput() {
	return userInput;
}

void GUI::recordMouseInput() {
	if (io->MousePos.x != userInput.currentMouseX) {
		userInput.lastMouseX = userInput.currentMouseX;
		userInput.currentMouseX = io->MousePos.x;
		userInput.mouseXMovement = mouseSensitivity * (userInput.currentMouseX - userInput.lastMouseX);
	}

	if (io->MousePos.y != userInput.currentMouseY) {
		userInput.lastMouseY = userInput.currentMouseY;
		userInput.currentMouseY = io->MousePos.y;
		userInput.mouseYMovement = mouseSensitivity * (userInput.currentMouseY - userInput.lastMouseY);
	}
}

void GUI::recordKeyboardInput() {
	userInput.w = keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_W];
	userInput.a = keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_A];
	userInput.s = keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_S];
	userInput.d = keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_D];
}