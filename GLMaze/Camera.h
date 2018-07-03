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

	struct Matrices {
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 transformation;
	};

	Camera();
	~Camera();

	Parameters calcNextParameter(GUI::UserInput userInput);
	void setTo(Parameters parameter);

	Parameters getParameter();
	Matrices getMatrices();

private:
	Parameters parameters;
	Matrices matrices;

	void readConfig();
};

