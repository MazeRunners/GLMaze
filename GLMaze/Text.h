#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H 

#include <glm/glm.hpp>

#include <map>

#include "GLShader.h"

struct Character {
	unsigned int     TextureID;     // 字形纹理的ID
	glm::ivec2 Size;       // 字形大小
	glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
	unsigned int     Advance;       // 原点距下一个字形原点的距离
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