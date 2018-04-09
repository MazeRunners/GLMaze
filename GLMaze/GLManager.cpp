#include <glad/glad.h>
#include "GLManager.h"
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

GLManager::GLManager() {
	glInit();
	glEnable(GL_DEPTH_TEST);
}

void GLManager::glInit() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "CG_HW5", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	compileAndLinkShaderProgram();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

void GLManager::setTransformation(glm::mat4 transformation) {
	this->transformation = transformation;
}

void GLManager::glRender() {
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);

	transform();

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
}

GLFWwindow * GLManager::getWindow() {
	return window;
}

bool GLManager::windowShouldClose() {
	return glfwWindowShouldClose(window);
}

void GLManager::pollEvents() {
	glfwPollEvents();
}

void GLManager::swapBuffer() {
	glfwSwapBuffers(window);
}

GLManager::~GLManager() {
	glfwTerminate();
}

void GLManager::transform() {
	unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transformation");
	glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformation));
}

std::string GLManager::readShaderSourceCode(const char * fileName) {
	std::ifstream shaderSourceFile(fileName);
	std::string shaderCode((std::istreambuf_iterator<char>(shaderSourceFile)),
		std::istreambuf_iterator<char>());
	return shaderCode;
}

void GLManager::compileAndLinkShaderProgram() {
	std::string vertexCode, fragmentCode;
	vertexCode = readShaderSourceCode("shader.vert");
	fragmentCode = readShaderSourceCode("shader.frag");
	const char * vertexShaderSource = vertexCode.c_str();
	const char *fragmentShaderSource = fragmentCode.c_str();

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
