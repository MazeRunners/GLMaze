#include "Lighting.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "Config.h"

Lighting::Lighting() {
	initShadowBuffer();
	readConfig();
}

void Lighting::readConfig() {
	Config cameraConfig("./config/lighting.cfg");

	mConfig.position = cameraConfig.getVec3("position");
	mConfig.center = cameraConfig.getVec3("center");
	mConfig.up = cameraConfig.getVec3("up");

	mConfig.left = cameraConfig.getFloat("left");
	mConfig.right = cameraConfig.getFloat("right");
	mConfig.bottom = cameraConfig.getFloat("bottom");
	mConfig.top = cameraConfig.getFloat("top");
	mConfig.zNear = cameraConfig.getFloat("zNear");
	mConfig.zFar = cameraConfig.getFloat("zFar");
}

Lighting::~Lighting() {}

void Lighting::initShadowBuffer() {
	glGenFramebuffers(1, &shadowDepthFBO);
	glGenTextures(1, &shadowDepthMap);

	glBindTexture(GL_TEXTURE_2D, shadowDepthFBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1280, 720, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
		shadowDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Lighting::calculateLightSpaceTransformation() {
	lightSpace.position = mConfig.position;

	lightSpace.projection = glm::ortho(mConfig.left, mConfig.right,
		mConfig.bottom, mConfig.top,
		mConfig.zNear, mConfig.zFar);

	lightSpace.view = glm::lookAt(lightSpace.position, mConfig.center, mConfig.up);

	lightSpace.transformation = lightSpace.projection * lightSpace.view;
}

void Lighting::beginShadowCalc() {
	shadowShader.use();
	shadowShader.setMat4("lightSpaceTransformation", lightSpace.transformation);

	glViewport(0, 0, 1280, 720);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Lighting::endShadowCalc() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

unsigned int Lighting::getShadowMap() { return shadowDepthMap; }

Lighting::LightSpace Lighting::getLightSpace() { return lightSpace; }

GLShader* Lighting::getShadowShader() { return &shadowShader; }
