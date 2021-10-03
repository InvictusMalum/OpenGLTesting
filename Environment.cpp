#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stdlib.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include"Environment.h"

using namespace std;

Environment::Environment(int SQUARES_WIDTH_, int SQUARES_HEIGHT_)
{
	/*vertices = new GLfloat[(int64_t)VERTS_WIDTH * VERTS_HEIGHT * 3];
	
	nodes = new bool*[NODES_HEIGHT];
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		nodes[i] = new bool[NODES_WIDTH];
	}
	
	oldNodes = new bool*[NODES_HEIGHT];
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		oldNodes[i] = new bool[NODES_WIDTH];
	}
	
	nodeIndices = new GLuint[(int64_t)SQUARES_HEIGHT * SQUARES_WIDTH * 6];
	indices = new GLuint[(int64_t)VERTS_HEIGHT * VERTS_WIDTH * 3];*/
}

void Environment::generateVertices()
{
	for (int i = 0; i < VERTS_HEIGHT; i++)
	{
		for (int j = 0; j < VERTS_WIDTH; j++)
		{
			vertices[(i * VERTS_WIDTH + j) * 3] = (GLfloat)(j) / (VERTS_WIDTH - 1) * 2 - 1;
			vertices[(i * VERTS_WIDTH + j) * 3 + 1] = -((GLfloat)(i) / (VERTS_HEIGHT - 1) * 2 - 1);
			vertices[(i * VERTS_WIDTH + j) * 3 + 2] = 0;
		}
	}
}

int Environment::getSurroundingOnNodes(int i, int j)
{
	int count = 0;
	for (int neighborI = i - 1; neighborI <= i + 1; neighborI++)
	{
		for (int neighborJ = j - 1; neighborJ <= j + 1; neighborJ++)
		{
			if (neighborI != i || neighborJ != j)
			{
				if (neighborI >= 0 && neighborI < NODES_HEIGHT && neighborJ >= 0 && neighborJ < NODES_WIDTH)
				{
					if (oldNodes[neighborI][neighborJ])
					{
						count++;
					}
				} else
				{
					count++;
				}
			}
		}
	}
	return count;
}

void Environment::smoothMap()
{
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		for (int j = 0; j < NODES_WIDTH; j++)
		{
			oldNodes[i][j] = nodes[i][j];
		}
	}

	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		for (int j = 0; j < NODES_WIDTH; j++)
		{
			int count = getSurroundingOnNodes(i, j);
			if (count > 4)
			{
				nodes[i][j] = true;
			} else if (count < 4)
			{
				nodes[i][j] = false;
			};
		};
	};
}

void Environment::generateNodeMap()
{
	srand(time(NULL));
	int percentOn = 43;
	int outLayer = 0;
	// Generating random nodes
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		for (int j = 0; j < NODES_WIDTH; j++)
		{
			if (i == 0 || j == 0 || i == NODES_HEIGHT - 1 || j == NODES_WIDTH - 1 || rand() % 100 + 1 <= percentOn)
			{
				nodes[i][j] = true;
			} else
			{
				nodes[i][j] = false;
			}
		}
	}
	// Smoothing nodes
	for (int i = 0; i < 10; i++)
	{
		smoothMap();
	}
}

int Environment::getSquares()
{

	return 4;
}

void Environment::marchSquares()
{
	
	int squareCombs[16][5][3] = {
	{{0}, { 0,0,0 }}, // 0
	{{1}, {0,1,VERTS_WIDTH}}, // 1
	{{1}, {1,2,VERTS_WIDTH + 2}}, // 2
	{{2}, {0,VERTS_WIDTH + 2,VERTS_WIDTH},{0,2,VERTS_WIDTH + 2}}, // 3
	{{1}, {VERTS_WIDTH,2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH}}, // 4
	{{2}, {0,1,2 * VERTS_WIDTH},{1,2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH}}, // 5
	{{4}, {1,2,VERTS_WIDTH},{2,2 * VERTS_WIDTH,VERTS_WIDTH},{2,VERTS_WIDTH + 2,2 * VERTS_WIDTH},{VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH}}, // 6
	{{3}, {0,2,VERTS_WIDTH + 2},{0,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1},{0,2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH}}, // 7
	{{1}, {VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1}}, // 8
	{{4}, {0,1,VERTS_WIDTH + 2},{0,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2},{0,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1},{0,2 * VERTS_WIDTH + 1,VERTS_WIDTH}}, // 9
	{{2}, {1,2,2 * VERTS_WIDTH + 1},{2,2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH + 2}}, // 10
	{{3}, {0,2,VERTS_WIDTH},{2,2 * VERTS_WIDTH + 1,VERTS_WIDTH},{2,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1}}, // 11
	{{2}, {VERTS_WIDTH,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2},{VERTS_WIDTH,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH}}, // 12
	{{3}, {0,1,2 * VERTS_WIDTH},{1,VERTS_WIDTH + 2,2 * VERTS_WIDTH},{VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH}}, // 13
	{{3}, {1,2,2 * VERTS_WIDTH + 2},{1,2 * VERTS_WIDTH + 2,VERTS_WIDTH},{VERTS_WIDTH,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH}}, // 14
	{{2}, {0,2,2 * VERTS_WIDTH + 2},{0,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH}}, // 15
	};

	// Index for next vertex, starts at 0, increments by 1 whenever a vertex is made.
	int nextOpen = 0;
	// Making Tris using marching Squares
	uint8_t code = 0;
	// March squares
	for (int i = 0; i < SQUARES_HEIGHT; i++)
	{
		for (int j = 0; j < SQUARES_WIDTH; j++)
		{
			code = 0;
			if (nodes[i][j])
				code += 1;
			if (nodes[i][j + 1])
				code += 2;
			if (nodes[i + 1][j])
				code += 4;
			if (nodes[i + 1][j + 1])
				code += 8;

			GLuint startVert = 2 * (i * VERTS_WIDTH + j);
			if (code != 0)
			{
				for (int tri = 1; tri <= squareCombs[code][0][0]; tri++)
				{
					indices[nextOpen] = startVert + squareCombs[code][tri][0];
					indices[nextOpen + 1] = startVert + squareCombs[code][tri][1];
					indices[nextOpen + 2] = startVert + squareCombs[code][tri][2];
					nextOpen += 3;
				}
			}
		}
	}
	numVertices = nextOpen;
}

void Environment::destroy()
{
	/*delete[] vertices;

	for (int i = 0; i < NODES_WIDTH; i++)
	{
		delete[] nodes[i];
		nodes[i] = NULL;
	}
	delete[] nodes;
	nodes = NULL;

	for (int i = 0; i < NODES_WIDTH; i++)
	{
		delete[] oldNodes[i];
		oldNodes[i] = NULL;
	}
	delete[] oldNodes;
	oldNodes = NULL;

	delete[] nodeIndices;
	delete[] indices;*/
}