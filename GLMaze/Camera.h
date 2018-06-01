#pragma once
#include <glm/glm.hpp>

#include "GUI.h"

class Camera {
public:
	struct Parameters {
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;

		float yaw = 0.0f;
		float pitch = 0.0f;

		float fovy;
		float aspect;
		float z_near;
		float z_far;
	};

	Camera(Parameters parameter);
	~Camera();

	void translateCamera(GUI::UserInput userInput);
	void moveWithUser(GUI::UserInput userInput);
	glm::mat4 getViewTransformation();
	Parameters getParameter();

private:
	Parameters parameters;
	glm::mat4 viewTransformation;
};

