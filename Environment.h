#pragma once

#ifndef Environment_H
#define Environment_H

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

using namespace std;

class Environment
{
public:
	Environment(int SQUARES_WIDTH_, int SQUARES_HEIGHT_);
	void GenerateVertices();
	int GetSurroundingOnNodes(int i, int j);
	void SmoothMap();
	void GenerateNodeMap();
	void MarchSquares();
	
	void GenerateShaders();
	void Draw();
	void ShaderClean();
	
	void Destroy();
	
	int numVertices = NULL;

	Shader shaderProgram;
	VAO VAO1;
	VBO VBO1;
	EBO	EBO1;
	Shader shaderProgram2;
	VAO VAO2;
	EBO EBO2;


	int SQUARES_WIDTH;
	int SQUARES_HEIGHT;

	int NODES_WIDTH;
	int NODES_HEIGHT;

	int VERTS_WIDTH;
	int VERTS_HEIGHT;

	GLfloat* vertices = NULL;
	bool** nodes = NULL;
	bool** oldNodes = NULL;
	//GLuint* nodeIndices = NULL;
	GLuint* lineIndices = NULL;
	GLuint* indices = NULL;
};

#endif
