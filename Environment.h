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
	int GetSquares();
	void MarchSquares();
	
	void GenerateShaders();
	void Draw();
	void ShaderClean();
	
	void Destroy();
	
	int numVertices = NULL;

	static const int SQUARES_WIDTH = 300;
	static const int SQUARES_HEIGHT = 200;

	static const int NODES_WIDTH = SQUARES_WIDTH + 1;
	static const int NODES_HEIGHT = SQUARES_HEIGHT + 1;

	static const int VERTS_WIDTH = 2 * SQUARES_WIDTH + 1;
	static const int VERTS_HEIGHT = 2 * SQUARES_HEIGHT + 1;

	GLfloat vertices[VERTS_WIDTH * VERTS_HEIGHT * 3];
	//bool nodes[NODES_HEIGHT][NODES_WIDTH];
	//bool oldNodes[NODES_HEIGHT][NODES_WIDTH];
	GLuint nodeIndices[SQUARES_HEIGHT * SQUARES_WIDTH * 6];
	GLuint indices[VERTS_HEIGHT * VERTS_WIDTH * 3];
	//GLuint lineIndices[VERTS_HEIGHT * VERTS_WIDTH * 6];


	Shader shaderProgram;
	VAO VAO1;
	VBO VBO1;
	EBO	EBO1;


	//int SQUARES_WIDTH;
	//int SQUARES_HEIGHT;

	//int NODES_WIDTH;
	//int NODES_HEIGHT;

	//int VERTS_WIDTH;
	//int VERTS_HEIGHT;

	//GLfloat* vertices = NULL;
	bool** nodes = NULL;
	bool** oldNodes = NULL;
	//GLuint* nodeIndices = NULL;
	//GLuint* indices = NULL;
};

#endif
