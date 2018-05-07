#pragma once

#include <libconfig.h++>
#include <glm/glm.hpp>

class GameConfig {
public:
	struct Parameters {
		glm::vec3 cameraPos;
		glm::vec3 cameraUp;
		glm::vec3 cameraFront;

		float fovy;
		float aspect;
		float z_near;
		float z_far;

		float mouseSensitivity;
		float keyboardSensitivity;
	};

	GameConfig();
	~GameConfig();
	const Parameters getParameters();

private:
	Parameters parameters;
	glm::vec3 readVec3(const libconfig::Setting& parent, const char* name);
};

