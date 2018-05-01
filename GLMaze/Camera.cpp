#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(Parameter parameter) {
	this->parameter = parameter;
}

Camera::~Camera() {
}

void Camera::moveCamera(GUI::UserInput userInput) {
	parameter.yaw += userInput.mouseXMovement;
	parameter.pitch += userInput.mouseYMovement;

	if (parameter.pitch > 89.0f) {
		parameter.pitch = 89.0f;
	}
	if (parameter.pitch < -89.0f) {
		parameter.pitch = -89.0f;
	}

	float yaw = glm::radians(parameter.yaw);
	float pitch = glm::radians(parameter.pitch);

	glm::vec3 front;
	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);
	parameter.front = glm::normalize(front);

	float xMove = userInput.d - userInput.a;
	float zMove = userInput.w - userInput.s;

	parameter.position += xMove * glm::normalize(glm::cross(parameter.front, parameter.up));
	parameter.position += zMove * parameter.front;

	glm::mat4 transformaton(1.0f);
	transformaton = glm::lookAt(parameter.position, parameter.position + parameter.front, parameter.up) * transformaton;
	transformaton = glm::perspective(glm::radians(60.0f), 1.5f, 0.5f, -0.5f) * transformaton;
	viewTransformation = transformaton;
}

glm::mat4 Camera::getViewTransformation() {
	return viewTransformation;
}

Camera::Parameter Camera::getParameter() {
	return parameter;
}
