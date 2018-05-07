#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

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

class Mesh {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> texture);
	void draw(GLShader* shader, bool no_texture);

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	GLuint VAO, VBO, EBO;
	void setupMesh();
};