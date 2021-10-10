#pragma once

#ifndef Environment_H
#define Environment_H

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include"TrisSquares.h"
#include"DrawingData.h"
#include"NodeMap.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

using namespace std;

class Environment
{
public:
	Environment(int SQUARES_WIDTH_, int SQUARES_HEIGHT_);
	void GenerateVertices();
	
	void GenerateNodes();

	void MarchAllSquares();

	void GenerateShaders();
	void Draw();
	void ShaderClean();
	
	void Destroy();
	
	GLfloat* vertices = NULL;
	
	NodeMap nM;

	VBO VBO;

	DrawingData mainMesh = NULL;
	DrawingData allLines = NULL;
	DrawingData exteriorLines = NULL;
	DrawingData uniqueExteriorLines = NULL;

	DrawingData onNodes = NULL;
	DrawingData offNodes = NULL;

	int SQUARES_WIDTH;
	int SQUARES_HEIGHT;

	int NODES_WIDTH;
	int NODES_HEIGHT;

	int VERTS_WIDTH;
	int VERTS_HEIGHT;
	
	Square* squares = NULL;
};

#endif
