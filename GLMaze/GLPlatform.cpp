#include "GLPlatform.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLPlatform::GLPlatform() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "GLMaze", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);
}

GLPlatform::~GLPlatform() {
	glfwTerminate();
}

GLFWwindow * GLPlatform::getWindow() const {
	return window;
}