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
		unsigned int textureID; // ���������ID
		glm::ivec2 size; // ���δ�С
		glm::ivec2 bearing; // �ӻ�׼�ߵ�������/������ƫ��ֵ
		unsigned int advance; // ԭ�����һ������ԭ��ľ���
	};

	std::map<char, Character> Characters;

	unsigned int VAO, VBO;
	GLShader textShader = GLShader("./shader/textshader.vert", "./shader/textshader.frag");

	void init();
	void readConfig();
};