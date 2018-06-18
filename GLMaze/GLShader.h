#pragma once

#include <GL\glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <glad/glad.h>

class GLShader {
public:
	GLShader(const char* vertShaderSrc, const char* fragShaderSrc);
	GLShader(const GLchar* vertShaderSrc, const GLchar* fragShaderSrc, const GLchar* geometryPath);
	GLShader(const GLchar* vertShaderSrc, const GLchar* fragShaderSrc, const GLchar* geometryPath,
		const GLchar* varyings[], int count);


	~GLShader();

	void use();

	void setInt(const char* varName, int number);
	void setFloat(const char* varName, float number);
	void setVec3(const char* varName, glm::vec3 vector);
	void setMat4(const char* varName, glm::mat4 matrix);

private:
	const char* vertShaderSrc;
	const char* fragShaderSrc;

	GLuint program;

	std::string readShaderSourceCode(const char* file);
	int compileShader(const char* shaderSrc, int shaderType);
	int buildShaderProgram();

	GLint locateUniformVar(const char* varName);
};

