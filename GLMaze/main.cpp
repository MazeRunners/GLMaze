#include "GLManager.h"
#include "ImGUIManager.h"
#include "Camera.h"

int main() {
	GLManager glManager = GLManager();

	ImGUIManager imGUIManager = ImGUIManager(glManager.getWindow());

	glm::mat4 transformaton(1.0f);
	glManager.setTransformation(transformaton);

	Camera camera = Camera();

	while (!glManager.windowShouldClose()) {
		glManager.pollEvents();
		camera.handleUserInput();

		imGUIManager.showGUI();
		glManager.glRender();

		transformaton = camera.calcTransformaton();
		glManager.setTransformation(transformaton);

		imGUIManager.GUIRender();
		glManager.swapBuffer();
	}
	return 0;
}