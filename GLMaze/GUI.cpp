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
	else {
		userInput.mouseXMovement = 0.0f;
	}


	if (io->MousePos.y != userInput.currentMouseY) {
		userInput.lastMouseY = userInput.currentMouseY;
		userInput.currentMouseY = io->MousePos.y;
		userInput.mouseYMovement = mouseSensitivity * (userInput.currentMouseY - userInput.lastMouseY);
	}
	else {
		userInput.mouseYMovement = 0.0f;
	}

}

void GUI::recordKeyboardInput() {
	userInput.w = io->KeysDownDuration[GLFW_KEY_W] >= 0.0f ? keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_W] : 0.0f;
	userInput.a = io->KeysDownDuration[GLFW_KEY_A] >= 0.0f ? keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_A] : 0.0f;
	userInput.s = io->KeysDownDuration[GLFW_KEY_S] >= 0.0f ? keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_S] : 0.0f;
	userInput.d = io->KeysDownDuration[GLFW_KEY_D] >= 0.0f ? keyboardSensitivity * io->KeysDownDuration[GLFW_KEY_D] : 0.0f;
}