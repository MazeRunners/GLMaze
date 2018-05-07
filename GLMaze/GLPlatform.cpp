#include "GLPlatform.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLPlatform::GLPlatform() {
	initWindow();
	initBuffer();

	glEnable(GL_DEPTH_TEST);
}

GLPlatform::~GLPlatform() {
	glfwTerminate();
}

const GLPlatform::Context GLPlatform::getContext() {
	return context;
}

void GLPlatform::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	context.window = glfwCreateWindow(1280, 720, "GLMaze", NULL, NULL);
	glfwMakeContextCurrent(context.window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void GLPlatform::initShadowMap() {
	glBindTexture(GL_TEXTURE_2D, context.shadowDepthFBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1280, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, context.shadowDepthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, context.shadowDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLPlatform::initBuffer() {
	glGenFramebuffers(1, &context.shadowDepthFBO);
	glGenTextures(1, &context.shadowDepthMap);

	initShadowMap();
}