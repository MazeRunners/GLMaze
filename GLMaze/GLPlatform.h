#pragma once
#include <glad/glad.h>

struct GLFWwindow;

class GLPlatform {
public:
	struct Context {
		GLFWwindow * window;

		GLuint shadowDepthFBO;
		GLuint shadowDepthMap;
	};

	GLPlatform();
	~GLPlatform();
	const Context getContext();

private:
	Context context;

	void initWindow();
	void initBuffer();
	void initShadowMap();
};
