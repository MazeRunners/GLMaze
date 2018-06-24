#include "GLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>

GLShader::GLShader(const char* vertShaderSrc, const char* fragShaderSrc) {
	this->vertShaderSrc = vertShaderSrc;
	this->fragShaderSrc = fragShaderSrc;
	program = buildShaderProgram();
}

GLShader::~GLShader() {
	glDeleteProgram(program);
}

void GLShader::use() {
	glUseProgram(program);
}

GLint GLShader::locateUniformVar(const char* varName) {
	GLint varLoc = glGetUniformLocation(program, varName);
	if (varLoc < 0) {
		std::cout << varName << " not found!" << std::endl;
	}
	return varLoc;
}

void GLShader::setInt(const char* varName, int integer) {
	GLint intLoc = locateUniformVar(varName);
	glUniform1i(intLoc, integer);
}

void GLShader::setFloat(const char* varName, float number) {
	GLint floatLoc = locateUniformVar(varName);
	glUniform1f(floatLoc, number);
}

void GLShader::setVec3(const char* varName, glm::vec3 vec3) {
	GLint vec3Loc = locateUniformVar(varName);
	glUniform3fv(vec3Loc, 1, glm::value_ptr(vec3));
}

void GLShader::setMat4(const char* varName, glm::mat4 mat4) {
	GLint mat4Loc = locateUniformVar(varName);
	glUniformMatrix4fv(mat4Loc, 1, GL_FALSE, glm::value_ptr(mat4));
}

std::string GLShader::readShaderSourceCode(const char * shaderFile) {
	std::ifstream fileStream(shaderFile);
	std::string shaderString((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	return shaderString;
}

int GLShader::compileShader(const char * shaderFile, int shaderType) {
	std::string shaderCode = readShaderSourceCode(shaderFile);
	const char * shaderSrc = shaderCode.c_str();
	int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);

	// output compilation error
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	return shader;
}

int GLShader::buildShaderProgram() {
	int vertexShader = compileShader(vertShaderSrc, GL_VERTEX_SHADER);
	int fragmentShader = compileShader(fragShaderSrc, GL_FRAGMENT_SHADER);

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}