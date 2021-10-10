#pragma once

#ifndef DRAWINGDATA
#define DRAWINGDATA

using namespace std;

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include <stdlib.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class DrawingData
{
public:
	DrawingData(int64_t indicesLength);
	void AddIndex(GLuint num);
	void Generate(VBO VBO, const char* vertShader, const char* fragShader, int sizeMult);
	void ShaderClean();
	
	GLuint* indices = NULL;
	int numVerts = NULL;
	Shader shaderProgram;
	VAO VAO;
	EBO	EBO;
};

#endif