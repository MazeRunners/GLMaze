#pragma once
#include <glm/glm.hpp>

#include "GLShader.h"

class Lighting {
public:
	Lighting();
	~Lighting();

	struct LightSpace {
		glm::vec3 position;
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 transformation;
	};

	void initShadowBuffer();
	void calculateLightSpaceTransformation();
	void beginShadowCalc();
	void endShadowCalc();

	unsigned int getShadowMap();
	LightSpace getLightSpace();
	GLShader* getShadowShader();

private:
	LightSpace lightSpace;
	unsigned int shadowDepthFBO;
	unsigned int shadowDepthMap;
	GLShader shadowShader =
		GLShader("./shader/shadow.vert", "./shader/shadow.frag");
	struct {
		glm::vec3 position;
		glm::vec3 center;
		glm::vec3 up;
		float left;
		float right;
		float bottom;
		float top;
		float zNear;
		float zFar;
	} mConfig;
	void readConfig();
};
