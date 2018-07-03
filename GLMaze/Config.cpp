#include "Config.h"

Config::Config(const char* file) {
	config.readFile(file);
}

Config::~Config() {
}

int Config::getInt(const char * name) {
	int result;
	config.getRoot().lookupValue(name, result);
	return result;
}

float Config::getFloat(const char * name) {
	float result;
	config.getRoot().lookupValue(name, result);
	return result;
}

std::string Config::getString(const char * name) {
	std::string result;
	config.getRoot().lookupValue(name, result);
	return result;
}

glm::vec3 Config::getVec3(const char* name) {
	const libconfig::Setting& vec3 = config.getRoot()[name];

	float scalar1 = vec3[0];
	float scalar2 = vec3[1];
	float scalar3 = vec3[2];

	return glm::vec3(scalar1, scalar2, scalar3);
}
