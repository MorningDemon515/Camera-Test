#ifndef SHADER
#define SHADER

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <glad\glad.h>

class ShaderFromSource
{
public:
	ShaderFromSource(const char* VertexShaderSource,const char* FragmentShaderSource);
	~ShaderFromSource();

	void Use();

	int GetShaderSourceUniform(const char* uniform);

private:
	GLuint VertexShader, FragmentShader, ShaderProgram;
};

class ShaderFromFile
{
public:
	ShaderFromFile(const char* VertexShaderSourceFile, const char* FragmentShaderSourceFile);
	~ShaderFromFile();

	void Use();

	int GetShaderSourceUniform(const char* uniform);

private:
	GLuint VertexShader, FragmentShader, ShaderProgram;
};

#endif 