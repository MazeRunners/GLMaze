#include "GameConfig.h"

GameConfig::GameConfig() {
	libconfig::Config config;
	config.readFile("./config/config.cfg");

	const libconfig::Setting& root = config.getRoot();

	const libconfig::Setting& camera = root["configuration"]["camera"];
	parameters.cameraPos = readVec3(camera, "position");
	parameters.cameraUp = readVec3(camera, "up");
	parameters.cameraFront = readVec3(camera, "front");

	camera.lookupValue("fovy", parameters.fovy);
	camera.lookupValue("aspect", parameters.aspect);
	camera.lookupValue("near", parameters.z_near);
	camera.lookupValue("far", parameters.z_far);

	const libconfig::Setting& control = root["configuration"]["control"];
	control.lookupValue("mouseSensitivity", parameters.mouseSensitivity);
	control.lookupValue("keyboardSensitivity", parameters.keyboardSensitivity);
}

GameConfig::~GameConfig() {
}

const GameConfig::Parameters GameConfig::getParameters() {
	return parameters;
}

glm::vec3 GameConfig::readVec3(const libconfig::Setting& parent, const char* name) {
	const libconfig::Setting& vec3 = parent[name];

	float scalar1 = vec3[0];
	float scalar2 = vec3[1];
	float scalar3 = vec3[2];

	return glm::vec3(scalar1, scalar2, scalar3);
}
