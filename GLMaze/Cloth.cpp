#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <iostream>

#include "Cloth.h"
#include "Config.h"

using std::vector;

Cloth::Cloth() {
	readConfig();

	restLen[0] = gridWidth;               // structural
	restLen[1] = gridWidth * pow(2, 0.5); // shear
	restLen[2] = gridWidth * 2;           // flexion

	createClothVertex();
	initBuffers();
	loadTexture(texturePath);
}

Cloth::~Cloth() {
}

void Cloth::render(glm::vec3 cameraPos, glm::mat4 viewTransformation, Lighting* lighting) {
	clothShader.use();
	clothShader.setVec3("viewPos", cameraPos);
	clothShader.setVec3("lightPos", lighting->getLightSpace().position);
	clothShader.setMat4("viewTransformation", viewTransformation);
	//clothShader.setMat4("lightSpaceTransformation", lighting->getLightSpace().transformation);
	// clothShader->setVec3("clothColor", glm::vec3(1.0f, 0, 0));

	for (int i = 0; i < height + 1; i++) {
		for (int j = 1; j < width + 1; j++) {
			updateForceFusion(i, j);
		}
	}

	updateVertexPosition();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	renderClothPlane();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Cloth::processInput(GLFWwindow * window) {
}

void Cloth::readConfig() {
	Config config("./config/cloth.cfg");

	gridWidth = config.getFloat("gridWidth");
	width = config.getInt("width");
	height = config.getInt("height");

	glm::vec3 stiffVec3 = config.getVec3("stiff");

	stiff[0] = stiffVec3[0]; // structural
	stiff[1] = stiffVec3[1]; // shear
	stiff[2] = stiffVec3[2]; // flexion  

	coefG = config.getFloat("coefG");
	coefD = config.getFloat("coefD");
	coefV = config.getFloat("coefV");

	coefFluid = config.getVec3("coefFluid");
}

void Cloth::loadTexture(const char * path) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Cloth::createClothVertex() {
	// vertices
	cVers = vector<ClothVertex>((width + 1) * (height + 1));
	float x = 0.0f - (float)width * gridWidth * 0.5;
	float y = 0.0f - (float)height * gridWidth * 0.5;
	for (int i = 0; i < height + 1; i++) {
		x = 0.0f - (float)width * gridWidth * 0.5;
		for (int j = 0; j < width + 1; j++) {
			cVers[i * (width + 1) + j].vPos = glm::vec3(6.0, y + 4.0f, x - 3.0f);
			cVers[i * (width + 1) + j].vTex = glm::vec2((float)i / (float)height, (float)j / (float)width);
			x += gridWidth;
			initClothVertex(i, j);
		}
		y += gridWidth;
	}

	// indices for EBO
	indices = vector<unsigned int>(width * height * 6);
	int k = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			indices[k] = i * (width + 1) + j;
			indices[k + 1] = indices[k] + 1;
			indices[k + 2] = indices[k] + width + 1;
			indices[k + 3] = indices[k + 1];
			indices[k + 4] = indices[k + 2];
			indices[k + 5] = indices[k + 2] + 1;
			k += 6;
		}
	}
}

void Cloth::initClothVertex(int i, int j) {
	int index = (width + 1) * i + j;
	cVers[index].fSpring = glm::vec3(0, 0, 0);
	cVers[index].fGravity = calGravityForce(i, j);
	cVers[index].fDamping = glm::vec3(0, 0, 0);
	cVers[index].fViscous = glm::vec3(0, 0, 0);
}

void Cloth::initBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ClothVertex) * cVers.size(), &cVers[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), (void*)offsetof(ClothVertex, vPos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), (void*)offsetof(ClothVertex, vNor));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ClothVertex), (void*)offsetof(ClothVertex, vTex));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Cloth::updateVertexPosition() {
	int index;
	glm::vec3 newVel;
	for (int i = 0; i < height + 1; i++) {
		for (int j = 1; j < width + 1; j++) {
			index = (width + 1) * i + j;
			newVel = cVers[index].vVel + 0.2f * calAccelaration(i, j);
			cVers[index].vPos += (newVel + cVers[index].vVel) * 0.2f * 0.0005f;
			cVers[index].vVel = newVel;
		}
	}
}

void Cloth::renderClothPlane() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ClothVertex) * cVers.size(), &cVers[0], GL_DYNAMIC_DRAW);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

glm::vec3 Cloth::calNormal(int i, int j) {
	if (i == height) {
		i--;
	}
	if (j == 0) {
		j++;
	}

	int index = i * (width + 1) + j;
	cVers[index].vNor = glm::normalize(glm::cross(
		cVers[index].vPos - cVers[index - 1].vPos,
		cVers[index].vPos - cVers[index + width + 1].vPos
	));
	return cVers[index].vNor;
}

glm::vec3 Cloth::calSpringForceBetween(glm::vec3 p, glm::vec3 q, float k, float l) {
	float distance = glm::distance(p, q);
	if (0.0000001 - fabs(l - distance) > 0) distance = l;
	glm::vec3 force = k * (l - distance) * (p - q) / distance;
	return force;

}

glm::vec3 Cloth::calSpringForce(int i, int j) {
	int index = i * (width + 1) + j;
	// 不计算固定位置
	if (j == 0) {
		cVers[index].fSpring = glm::vec3(0, 0, 0);
	}
	else {
		cVers[index].fSpring = calSpringForceStruct(i, j)
			+ calSpringForceShear(i, j)
			+ calSpringForceFlexion(i, j);
	}
	return cVers[index].fSpring;
}

glm::vec3 Cloth::calGravityForce(int i, int j) {
	int index = i * (width + 1) + j;
	float Fy = -cVers[index].mass * coefG;
	cVers[index].fGravity = glm::vec3(0, Fy, 0);
	return cVers[index].fGravity;
}

glm::vec3 Cloth::calDampingForce(int i, int j) {
	int index = i * (width + 1) + j;
	cVers[index].fDamping = -coefD * cVers[index].vVel;
	return cVers[index].fDamping;
}

glm::vec3 Cloth::calViscousForce(int i, int j) {
	int index = i * (width + 1) + j;
	calNormal(i, j);
	cVers[index].fViscous = -glm::vec3((0.5f - rand() / float(RAND_MAX)) * 4.0f, 0, (0.5f - rand() / float(RAND_MAX)) * 4.0f)
		+ coefV * (cVers[index].vNor * (coefFluid - cVers[index].vVel)) * cVers[index].vNor;
	return cVers[index].fViscous;
}

glm::vec3 Cloth::calSpringForceStruct(int i, int j) {
	int index = i * (width + 1) + j;
	glm::vec3 Fstruct = glm::vec3(0, 0, 0);
	if (j != width) {
		Fstruct += calSpringForceBetween(cVers[index].vPos,
			cVers[index + 1].vPos,
			stiff[0],
			restLen[0]);
	}
	if (i != height) {
		Fstruct += calSpringForceBetween(cVers[index].vPos,
			cVers[index + width + 1].vPos,
			stiff[0],
			restLen[0]);
	}
	if (i != 0) {
		Fstruct += calSpringForceBetween(cVers[index].vPos,
			cVers[index - width - 1].vPos,
			stiff[0],
			restLen[0]);
	}
	if (j != 0) {
		Fstruct += calSpringForceBetween(cVers[index].vPos,
			cVers[index - 1].vPos,
			stiff[0],
			restLen[0]);
	}
	return Fstruct;
}

glm::vec3 Cloth::calSpringForceShear(int i, int j) {
	int index = i * (width + 1) + j;
	glm::vec3 Fshear = glm::vec3(0, 0, 0);
	if (j != width) {
		if (i != 0) {
			Fshear += calSpringForceBetween(cVers[index].vPos,
				cVers[index - width].vPos,
				stiff[1],
				restLen[1]);
		}
		if (i != height) {
			Fshear += calSpringForceBetween(cVers[index].vPos,
				cVers[index + width + 2].vPos,
				stiff[1],
				restLen[1]);
		}
	}
	if (j != 0) {
		if (i != height) {
			Fshear += calSpringForceBetween(cVers[index].vPos,
				cVers[index + width].vPos,
				stiff[1],
				restLen[1]);
		}
		if (i != 0) {
			Fshear += calSpringForceBetween(cVers[index].vPos,
				cVers[index - width - 2].vPos,
				stiff[1],
				restLen[1]);
		}
	}
	return Fshear;
}

glm::vec3 Cloth::calSpringForceFlexion(int i, int j) {
	int index = i * (width + 1) + j;
	glm::vec3 Fflexion = glm::vec3(0, 0, 0);
	if (j < width - 1) {
		Fflexion += calSpringForceBetween(cVers[index].vPos,
			cVers[index + 2].vPos,
			stiff[2],
			restLen[2]);
	}
	if (j > 1) {
		Fflexion += calSpringForceBetween(cVers[index].vPos,
			cVers[index - 2].vPos,
			stiff[2],
			restLen[2]);
	}
	if (i < height - 1) {
		Fflexion += calSpringForceBetween(cVers[index].vPos,
			cVers[index + width * 2 + 2].vPos,
			stiff[2],
			restLen[2]);
	}
	if (i > 1) {
		Fflexion += calSpringForceBetween(cVers[index].vPos,
			cVers[index - width * 2 - 2].vPos,
			stiff[2],
			restLen[2]);
	}
	return Fflexion;
}

void Cloth::addManualForce() {
}

glm::vec3 Cloth::updateForceFusion(int i, int j) {
	calSpringForce(i, j);
	calDampingForce(i, j);
	calViscousForce(i, j);
	int index = (width + 1) * i + j;
	cVers[index].fFuse = cVers[index].fDamping
		+ cVers[index].fGravity
		+ cVers[index].fViscous
		+ cVers[index].fSpring;
	return cVers[index].fFuse;
}

glm::vec3 Cloth::calAccelaration(int i, int j) {
	int index = (width + 1) * i + j;
	glm::vec3 acc = cVers[index].fFuse / cVers[index].mass;
	return acc;
}
