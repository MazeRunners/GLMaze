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
	GLuint     TextureID;     // ���������ID
	glm::ivec2 Size;       // ���δ�С
	glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
	GLuint     Advance;       // ԭ�����һ������ԭ��ľ���
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