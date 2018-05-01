#pragma once
#include <glm/glm.hpp>

#include "GUI.h"

class Camera {
public:
	struct Parameter {
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;

		float yaw = 0.0f;
		float pitch = 0.0f;
	};

	Camera(Parameter parameter);
	~Camera();

	void moveCamera(GUI::UserInput userInput);
	glm::mat4 getViewTransformation();
	Parameter getParameter();

private:
	Parameter parameter;
	glm::mat4 viewTransformation;
};

