#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(Parameters parameter) {
	this->parameters = parameter;
}

Camera::~Camera() {
}

Camera::Parameters Camera::calcNextParameter(GUI::UserInput userInput) {
	Parameters next = parameters;

	next.yaw = next.yaw + userInput.mouseXMovement;
	next.pitch = next.pitch + userInput.mouseYMovement;

	if (next.pitch > 89.0f) {
		next.pitch = 89.0f;
	}
	if (next.pitch < -89.0f) {
		next.pitch = -89.0f;
	}

	float yaw = glm::radians(next.yaw);
	float pitch = glm::radians(next.pitch);

	glm::vec3 front;
	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);
	next.front = glm::normalize(front);

	float xMove = userInput.d - userInput.a;
	float zMove = userInput.w - userInput.s;

	next.position = next.position + xMove * glm::normalize(glm::cross(next.front, next.up));
	next.position = next.position + zMove * next.front;
	next.position.y = 0.1;

	return next;
}

void Camera::moveTo(Camera::Parameters next) {
	parameters = next;
	glm::mat4 transformation(1.0f);

	// view matrix
	transformation = glm::lookAt(parameters.position, parameters.position + parameters.front, parameters.up) * transformation;

	// projection * view
	transformation = glm::perspective(glm::radians(parameters.fovy), parameters.aspect,
		parameters.z_near, parameters.z_far) * transformation;

	viewTransformation = transformation;
}

glm::mat4 Camera::getViewTransformation() {
	return viewTransformation;
}

Camera::Parameters Camera::getParameter() {
	return parameters;
}
