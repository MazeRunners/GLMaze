#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H 

#include <string>
#include <map>

#include "GLShader.h"

class Text {
public:
	Text();
	~Text();
	void renderText();

private:
	struct {
		std::string text;
		float start_x;
		float start_y;
		float scale;
		glm::vec3 color;
	} mConfig;

	FT_Library ft;
	FT_Face face;

	struct Character {
		unsigned int textureID; // 字形纹理的ID
		glm::ivec2 size; // 字形大小
		glm::ivec2 bearing; // 从基准线到字形左部/顶部的偏移值
		unsigned int advance; // 原点距下一个字形原点的距离
	};

	std::map<char, Character> Characters;

	unsigned int VAO, VBO;
	GLShader textShader = GLShader("./shader/textshader.vert", "./shader/textshader.frag");

	void init();
	void readConfig();
};