#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Config.h"

Camera::Camera() {
	readConfig();
}

void Camera::readConfig() {
	Config cameraConfig("./config/camera.cfg");

	parameters.position = cameraConfig.getVec3("position");
	parameters.up = cameraConfig.getVec3("up");
	parameters.front = cameraConfig.getVec3("front");

	parameters.fovy = cameraConfig.getFloat("fovy");
	parameters.aspect = cameraConfig.getFloat("aspect");
	parameters.z_near = cameraConfig.getFloat("z_near");
	parameters.z_far = cameraConfig.getFloat("z_far");
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
	next.position.y = 0.16;

	return next;
}

void Camera::setTo(Camera::Parameters next) {
	parameters = next;
	glm::mat4 transformation(1.0f);

	matrices.view = glm::lookAt(parameters.position, parameters.position + parameters.front, parameters.up) * transformation;
	matrices.projection = glm::perspective(glm::radians(parameters.fovy), parameters.aspect, parameters.z_near, parameters.z_far);
	matrices.transformation = matrices.projection * matrices.view;
}

Camera::Parameters Camera::getParameter() {
	return parameters;
}

Camera::Matrices Camera::getMatrices() {
	return matrices;
}
