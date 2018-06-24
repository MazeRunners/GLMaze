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

	Parameters calcNextParameter(GUI::UserInput userInput);
	void moveTo(Parameters parameter);

	glm::mat4 getView();
	glm::mat4 getProjection();
	glm::mat4 getViewTransformation();
	Parameters getParameter();

private:
	Parameters parameters;

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 viewTransformation;
};

