#pragma once
#include <libconfig.h++>
#include <glm/glm.hpp>

#include <string>

class Config {
public:
	Config(const char* file);
	~Config();

	int getInt(const char* name);
	float getFloat(const char* name);
	std::string getString(const char* name);
	glm::vec3 getVec3(const char* name);

private:
	libconfig::Config config;
};

