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
	void SetDrawingNodes();
	void RotateSelected(int n);

	void MarchAllSquares();

	void ToggleMesh();
	void ToggleLines();
	void ToggleExteriors();
	void ToggleUniques();

	void ToggleOns();
	void ToggleOffs();

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

	int selectedRegion = 2;

	int countdownMax = 1000;

	int rotationTimer = 0;
	int rotationMax = 500;

	bool renderMesh = true;
	int meshCountdown = 0;

	bool renderLines = false;
	int linesCountdown = 0;

	bool renderExteriors = false;
	int exteriorsCountdown = 0;

	bool renderUniques = false;
	int uniquesCountdown = 0;

	bool renderOns = false;
	int onsCountdown = 0;

	bool renderOffs = false;
	int offsCountdown = 0;

	int SQUARES_WIDTH;
	int SQUARES_HEIGHT;

	int NODES_WIDTH;
	int NODES_HEIGHT;

	int VERTS_WIDTH;
	int VERTS_HEIGHT;
	
	Square* squares = NULL;
};

#endif
