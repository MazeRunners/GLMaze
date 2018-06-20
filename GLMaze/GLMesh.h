#pragma once
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "GLShader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class GLMesh {
public:
	GLMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> texture);
	void draw(GLShader* shader, bool no_texture);

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int VAO, VBO, EBO;
	void setupMesh();
};