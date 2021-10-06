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

class Triangle
{
public:
	Triangle();
	void SetVerts(GLuint v1, GLuint v2, GLuint v3);

	GLuint v1;
	GLuint v2;
	GLuint v3;
};


class Square
{
public:
	Square();
	void SetCorners(GLuint c1_, GLuint c4_);
	void MarchSquare(bool** nodes, int squareCombs[16][5][3], int VERTS_WIDTH);
	GLuint* GetOutlineLines();
	
	Triangle* tris;
	int triNum;
	uint8_t code = 0;

	GLuint c1;
	GLuint c2;
	GLuint c3;
	GLuint c4;
};



class Environment
{
public:
	Environment(int SQUARES_WIDTH_, int SQUARES_HEIGHT_);
	void GenerateVertices();
	int GetSurroundingOnNodes(int i, int j);
	void SmoothMap();
	void GenerateNodeMap();
	
	void MarchAllSquares();
	void MarchSquares();
	
	void GenerateShaders();
	void Draw();
	void ShaderClean();
	
	void Destroy();
	
	int numMeshVerts = NULL;
	int numLineVerts = NULL;

	Shader shaderProgram;
	VAO VAO1;
	VBO VBO1;
	EBO	EBO1;
	Shader shaderProgram2;
	VAO VAO2;
	EBO EBO2;
	Shader shaderProgram3;
	VAO VAO3;
	EBO EBO3;

	int SQUARES_WIDTH;
	int SQUARES_HEIGHT;

	int NODES_WIDTH;
	int NODES_HEIGHT;

	int VERTS_WIDTH;
	int VERTS_HEIGHT;
	
	Square* squares = NULL;

	GLfloat* vertices = NULL;
	bool** nodes = NULL;
	bool** oldNodes = NULL;
	//GLuint* nodeIndices = NULL;
	GLuint* indices = NULL;
	GLuint* lineIndices = NULL;
	GLuint* exteriorLineIndices = NULL;
};

#endif
