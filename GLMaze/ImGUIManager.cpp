#include "ImGUIManager.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

ImGUIManager::ImGUIManager(GLFWwindow * window) {
	showEdit = true;
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
}

ImGUIManager::~ImGUIManager() {
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImGUIManager::showGUI() {
	ImGui_ImplGlfwGL3_NewFrame();

	//ImGui::Begin("Maze", &showEdit, ImGuiWindowFlags_AlwaysAutoResize);

	//ImGui::End();
}

void ImGUIManager::GUIRender() {
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}