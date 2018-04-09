#pragma once
#include <GLFW/glfw3.h>

class ImGUIManager {
public:
	ImGUIManager(GLFWwindow * window);
	~ImGUIManager();
	void showGUI();
	void GUIRender();

private:
	bool showEdit;
};

