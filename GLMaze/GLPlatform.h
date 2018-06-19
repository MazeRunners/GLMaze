#pragma once

struct GLFWwindow;

class GLPlatform {
public:
	struct Context {
		GLFWwindow * window;

		unsigned int shadowDepthFBO;
		unsigned int shadowDepthMap;
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
