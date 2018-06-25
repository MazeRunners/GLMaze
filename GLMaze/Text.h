#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H 

#include <glm/glm.hpp>

#include <map>

#include "GLShader.h"

struct Character {
	unsigned int     TextureID;     // ���������ID
	glm::ivec2 Size;       // ���δ�С
	glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
	unsigned int     Advance;       // ԭ�����һ������ԭ��ľ���
};



class Text {
public:
	Text();
	~Text();

	void init();
	void RenderText(GLShader &s, std::string text, float x, float y, float scale, glm::vec3 color);

private:
	std::map<char, Character> Characters;
	unsigned int VAO, VBO;
	FT_Library ft;
	FT_Face face;
};