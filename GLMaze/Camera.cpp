#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(Parameters parameter) {
	this->parameters = parameter;
}

Camera::~Camera() {
}

void Camera::moveCamera(GUI::UserInput userInput) {
	parameters.yaw += userInput.mouseXMovement;
	parameters.pitch += userInput.mouseYMovement;

	if (parameters.pitch > 89.0f) {
		parameters.pitch = 89.0f;
	}
	if (parameters.pitch < -89.0f) {
		parameters.pitch = -89.0f;
	}

	float yaw = glm::radians(parameters.yaw);
	float pitch = glm::radians(parameters.pitch);

	glm::vec3 front;
	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);
	parameters.front = glm::normalize(front);

	float xMove = userInput.d - userInput.a;
	float zMove = userInput.w - userInput.s;

	parameters.position += xMove * glm::normalize(glm::cross(parameters.front, parameters.up));
	parameters.position += zMove * parameters.front;

	glm::mat4 transformaton(1.0f);
	transformaton = glm::lookAt(parameters.position, parameters.position + parameters.front, parameters.up) * transformaton;
	transformaton = glm::perspective(glm::radians(parameters.fovy), parameters.aspect,
		parameters.z_near, parameters.z_far) * transformaton;
	viewTransformation = transformaton;
}

glm::mat4 Camera::getViewTransformation() {
	return viewTransformation;
}

Camera::Parameters Camera::getParameter() {
	return parameters;
}
