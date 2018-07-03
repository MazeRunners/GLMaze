#pragma once

struct GLFWwindow;

class GLPlatform {
public:
	GLPlatform();
	~GLPlatform();
	GLFWwindow * getWindow() const;

private:
	GLFWwindow * window;
};
