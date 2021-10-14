#pragma once

#ifndef TRISSQUARES_H
#define TRISSQUARES_H

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
	void MarchSquare(int** nodes, int*** squareCombs, int*** outLineCombs, int VERTS_WIDTH);
	GLuint* GetOutlineLines();

	int numOutVerts;
	GLuint* outVerts = new GLuint[8];

	Triangle* tris = new Triangle[4];
	int numTris;
	uint8_t code = 0;

	GLuint c1;
	GLuint c2;
	GLuint c3;
	GLuint c4;
};

#endif