#include "Cloth.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>

#include "Cloth.h"

using std::vector;

Cloth::Cloth(float gridWidth_, int width_, int height_, char* texturePath)
{
	gridWidth = gridWidth_;
	width = width_;
	height = height_;

	restLen[0] = gridWidth;               // structural
	restLen[1] = gridWidth * pow(2, 0.5); // shear
	restLen[2] = gridWidth * 2;           // flexion  
	stiff[0] = 400.0f; // structural
	stiff[1] = 250.0f; // shear
	stiff[2] = 250.0f; // flexion  
	Cg = 4.8f;
	Cd = 0.8f;
	Cv = 8.0f;

	Ufluid = glm::vec3(1.0f, 0.0f, 0.1f);

	lastCalcTime = (float)glfwGetTime();

	CreateClothVertex();
	InitBuffers();
	loadTexture(texturePath);
}

Cloth::~Cloth()
{
}

void Cloth::draw()
{
	UpdateVertexPosition();
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	RenderClothPlane();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Cloth::ProcessInput(GLFWwindow * window)
{
}

void Cloth::loadTexture(char * path)
{
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
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Cloth::CreateClothVertex()
{
	// vertices
	cVers = vector<ClothVertex> ((width + 1) * (height + 1));
	float x = 0.0f - (float)width * gridWidth * 0.5;
	float y = 0.0f - (float)height * gridWidth * 0.5;
	for (int i = 0; i < height + 1; i++) {
		x = 0.0f - (float)width * gridWidth * 0.5;
		for (int j = 0; j < width + 1; j++) {
			cVers[i * (width+1) + j].vPos = glm::vec3(x, y, 0);
			cVers[i * (width + 1) + j].vTex = glm::vec2((float)i / (float)height, (float)j / (float)width);
			x += gridWidth;
			InitClothVertex(i, j);
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

void Cloth::InitClothVertex(int i, int j)
{
	int index = (width + 1) * i + j;
	cVers[index].Fspring = glm::vec3(0, 0, 0);
	cVers[index].Fgravity = CalGravityForce(i, j);
	cVers[index].Fdamping = glm::vec3(0, 0, 0);
	cVers[index].Fviscous = glm::vec3(0, 0, 0);
}

void Cloth::InitBuffers()
{
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

void Cloth::UpdateVertexPosition()
{
	float t = (float)glfwGetTime();
	int index;
	glm::vec3 newVel;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width + 1; j++) {
			index = (width + 1) * i + j;
			newVel = cVers[index].vVel + 0.05f * 2.5f * CalAccelaration(i, j);
			cVers[index].vPos += (newVel + cVers[index].vVel) * 0.05f * 2.5f * 0.5f * 0.005f;
			cVers[index].vVel = newVel;
		}
	}
	lastCalcTime = t;
}

void Cloth::RenderClothPlane()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ClothVertex) * cVers.size(), &cVers[0], GL_DYNAMIC_DRAW);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

glm::vec3 Cloth::CalNormal(int i, int j)
{
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

glm::vec3 Cloth::CalSpringForceBetween(glm::vec3 p, glm::vec3 q, float k, float l)
{
	float distance = glm::distance(p, q);
	return k * (l - distance) * (p - q) / distance;
}

glm::vec3 Cloth::CalSpringForce(int i, int j)
{
	int index = i * (width + 1) + j;
	if (i == height) {
		cVers[index].Fspring = glm::vec3(0, 0, 0);
	}
	else {
		cVers[index].Fspring = CalSpringForceStruct(i, j)
			+ CalSpringForceShear(i, j)
			+ CalSpringForceFlexion(i, j);
	}
	return cVers[index].Fspring;
}

glm::vec3 Cloth::CalGravityForce(int i, int j)
{
	int index = i * (width + 1) + j;
	float Fy = -cVers[index].mass * Cg;
	cVers[index].Fgravity = glm::vec3(0, Fy, 0);
	return cVers[index].Fgravity;
}

glm::vec3 Cloth::CalDampingForce(int i, int j)
{
	int index = i * (width + 1) + j;
	cVers[index].Fdamping = -Cd * cVers[index].vVel;
	return cVers[index].Fdamping;
}

glm::vec3 Cloth::CalViscousForce(int i, int j)
{
	int index = i * (width + 1) + j;
	CalNormal(i, j);
	cVers[index].Fviscous = - glm::vec3((0.5f - rand() / float(RAND_MAX)) * 4.0f, 0, (0.5f - rand() / float(RAND_MAX)) * 16.0f)
		+ Cv * (cVers[index].vNor * (Ufluid - cVers[index].vVel)) * cVers[index].vNor;
	return cVers[index].Fviscous;
}

glm::vec3 Cloth::CalSpringForceStruct(int i, int j)
{
	int index = i * (width + 1) + j;
	glm::vec3 Fstruct = glm::vec3(0, 0, 0);
	if (j != width) {
		Fstruct += CalSpringForceBetween(cVers[index].vPos,
			cVers[index + 1].vPos,
			stiff[0],
			restLen[0]);
	}
	if (i != height) {
		Fstruct += CalSpringForceBetween(cVers[index].vPos,
			cVers[index + width + 1].vPos,
			stiff[0],
			restLen[0]);
	}
	if (i != 0) {
		Fstruct += CalSpringForceBetween(cVers[index].vPos,
			cVers[index - width - 1].vPos,
			stiff[0],
			restLen[0]);
	}
	if (j != 0) {
		Fstruct += CalSpringForceBetween(cVers[index].vPos,
			cVers[index - 1].vPos,
			stiff[0],
			restLen[0]);
	}
	return Fstruct;
}

glm::vec3 Cloth::CalSpringForceShear(int i, int j)
{
	int index = i * (width + 1) + j;
	glm::vec3 Fshear = glm::vec3(0, 0, 0);
	if (j != width) {
		if (i != 0) {
			Fshear += CalSpringForceBetween(cVers[index].vPos,
				cVers[index - width].vPos,
				stiff[1],
				restLen[1]);
		}
		if (i != height) {
			Fshear += CalSpringForceBetween(cVers[index].vPos,
				cVers[index + width + 2].vPos,
				stiff[1],
				restLen[1]);
		}
	}
	if (j != 0) {
		if (i != height) {
			Fshear += CalSpringForceBetween(cVers[index].vPos,
				cVers[index + width].vPos,
				stiff[1],
				restLen[1]);
		}
		if (i != 0) {
			Fshear += CalSpringForceBetween(cVers[index].vPos,
				cVers[index - width - 2].vPos,
				stiff[1],
				restLen[1]);
		}
	}
	return Fshear;
}

glm::vec3 Cloth::CalSpringForceFlexion(int i, int j)
{
	int index = i * (width + 1) + j;
	glm::vec3 Fflexion = glm::vec3(0, 0, 0);
	if (j < width - 1) {
		Fflexion += CalSpringForceBetween(cVers[index].vPos,
			cVers[index + 2].vPos,
			stiff[2],
			restLen[2]);
	}
	if (j > 1) {
		Fflexion += CalSpringForceBetween(cVers[index].vPos,
			cVers[index - 2].vPos,
			stiff[2],
			restLen[2]);
	}
	if (i < height - 1) {
		Fflexion += CalSpringForceBetween(cVers[index].vPos,
			cVers[index + width * 2 + 2].vPos,
			stiff[2],
			restLen[2]);
	}
	if (i > 1) {
		Fflexion += CalSpringForceBetween(cVers[index].vPos,
			cVers[index - width * 2 - 2].vPos,
			stiff[2],
			restLen[2]);
	}
	return Fflexion;
}

void Cloth::AddManualForce()
{
}

glm::vec3 Cloth::UpdateForceFusion(int i, int j)
{
	CalSpringForce(i, j);
	CalDampingForce(i, j);
	CalViscousForce(i, j);
	int index = (width + 1) * i + j;
	cVers[index].Ffuse = cVers[index].Fdamping
		+ cVers[index].Fgravity
		+ cVers[index].Fviscous
		+ cVers[index].Fspring;
	return cVers[index].Ffuse;
}

glm::vec3 Cloth::CalAccelaration(int i, int j)
{
	UpdateForceFusion(i, j);
	int index = (width + 1) * i + j;
	glm::vec3 acc = cVers[index].Ffuse / cVers[index].mass;
	return acc;
}
