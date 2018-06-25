#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <map>

#include "Camera.h"
#include "GLShader.h"

struct Character {
	GLuint     TextureID;     // 字形纹理的ID
	glm::ivec2 Size;       // 字形大小
	glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
	GLuint     Advance;       // 原点距下一个字形原点的距离
};



class Text {
public:
	Text();
	~Text();

	void init();
	void RenderText(GLShader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;
	FT_Library ft;
	FT_Face face;
};