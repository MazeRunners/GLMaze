#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>

class GLManager {
public:
	GLManager();
	void setTransformation(glm::mat4 transformation);
	void glRender();
	bool windowShouldClose();
	void pollEvents();
	void swapBuffer();
	~GLManager();
	GLFWwindow * getWindow();

private:
	GLFWwindow * window;
	int shaderProgram;
	unsigned int VAO;
	unsigned int VBO;

	glm::mat4 transformation;

	void glInit();
	std::string readShaderSourceCode(const char* fileName);
	void compileAndLinkShaderProgram();
	void transform();
};

