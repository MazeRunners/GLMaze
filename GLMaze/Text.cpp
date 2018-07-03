#include "Text.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "Camera.h"
#include "Config.h"

Text::Text() {
	init();
	readConfig();
}
Text::~Text() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Text::init() {
	// Set OpenGL options
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	if (FT_New_Face(ft, "./resource/font/Carolingia.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);  //���������С�����ʾ������Ҫ�������������ɶ�������

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //�����ֽڶ�������

	for (GLubyte c = 0; c < 128; c++) {
		// �����ַ������� 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// ��������
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// ��������ѡ��
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// �����ַ���֮��ʹ��
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::readConfig() {
	Config config("./config/text.cfg");

	mConfig.text = config.getString("text");
	mConfig.scale = config.getFloat("scale");
	mConfig.start_x = config.getFloat("start_x");
	mConfig.start_y = config.getFloat("start_y");
	mConfig.color = config.getVec3("color");
}

void Text::renderText() {
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

	// �����Ӧ����Ⱦ״̬
	textShader.use();
	textShader.setMat4("projection", projection);
	textShader.setVec3("textColor", mConfig.color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// �����ı������е��ַ�
	std::string::const_iterator c;
	float x = mConfig.start_x;
	float y = mConfig.start_y;
	for (c = mConfig.text.begin(); c != mConfig.text.end(); c++) {
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.bearing.x * mConfig.scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * mConfig.scale;
		GLfloat w = ch.size.x * mConfig.scale;
		GLfloat h = ch.size.y * mConfig.scale;

		// ��ÿ���ַ�����VBO
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		// ���ı����ϻ�����������
		glBindTexture(GL_TEXTURE_2D, ch.textureID);

		// ����VBO�ڴ������
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// �����ı���
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
		x += (ch.advance >> 6) * mConfig.scale; // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}