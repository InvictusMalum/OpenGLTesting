#include<iostream>
#include<set>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stdlib.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include"Environment.h"

using namespace std;

Triangle::Triangle()
{

}

void Triangle::SetVerts(GLuint v1_, GLuint v2_, GLuint v3_)
{
	v1 = v1_;
	v2 = v2_;
	v3 = v3_;
}

Square::Square()
{

}

void Square::SetCorners(GLuint c1_, GLuint c4_)
{
	c1 = c1_;
	c2 = c1_ + 2;
	c3 = c4_ - 2;
	c4 = c4_;
}

void Square::MarchSquare(bool** nodes, int squareCombs[16][5][3], int outLineCombs[16][3][2], int VERTS_WIDTH)
{
	GLuint j = c1 / 2 % VERTS_WIDTH;
	GLuint i = (c1 / 2 - j) / VERTS_WIDTH;

	code = 0;
	if (nodes[i][j])
		code += 1;
	if (nodes[i][j + 1])
		code += 2;
	if (nodes[i + 1][j])
		code += 4;
	if (nodes[i + 1][j + 1])
		code += 8;

	GLuint startVert = c1;
	if (code != 0)
	{
		numTris = squareCombs[code][0][0];
		tris = new Triangle[numTris];
		for (int tri = 1; tri <= numTris; tri++)
		{
			tris[tri-1].SetVerts(c1 + squareCombs[code][tri][0], c1 + squareCombs[code][tri][1], c1 + squareCombs[code][tri][2]);
		}

		if (code != 15)
		{
			int numLines = outLineCombs[code][0][0];
			numOutVerts = numLines * 2;
			outVerts = new GLuint[numOutVerts];

			int nextVert = 0;
			for (int i = 1; i <= numLines; i++)
			{
				outVerts[nextVert] = c1 + outLineCombs[code][i][0];
				outVerts[nextVert+1] = c1 + outLineCombs[code][i][1];
				nextVert += 2;
			}
		}
	}
}

GLuint* Square::GetOutlineLines()
{
	GLuint* out = new GLuint[(int64_t)numTris + 2];
	out[0] = tris[0].v1;
	out[1] = tris[0].v2;
	out[2] = tris[0].v3;
	for (int i = 1; i < numTris; i++)
	{
		out[2+i] = tris[i].v3;
	}
	return out;
}

Environment::Environment(int SQUARES_WIDTH_, int SQUARES_HEIGHT_)
{
	SQUARES_WIDTH = SQUARES_WIDTH_;
	SQUARES_HEIGHT = SQUARES_HEIGHT_;

	NODES_WIDTH = SQUARES_WIDTH + 1;
	NODES_HEIGHT = SQUARES_HEIGHT + 1;

	VERTS_WIDTH = 2 * NODES_WIDTH - 1;
	VERTS_HEIGHT = 2 * NODES_HEIGHT - 1;

	squares = new Square[(int64_t)SQUARES_WIDTH * SQUARES_HEIGHT];
	for (int i = 0; i < SQUARES_HEIGHT; i++)
	{
		for (int j = 0; j < SQUARES_WIDTH; j++)
		{
			squares[(int64_t)i * SQUARES_WIDTH + j].SetCorners(2*(i * VERTS_WIDTH + j), 2*(i * SQUARES_WIDTH + j) + 2*VERTS_WIDTH + 2);
		}
	}
	
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
	
	vertices = new GLfloat[(int64_t)VERTS_WIDTH * VERTS_HEIGHT*6];

	mainMesh = DrawingData((int64_t)VERTS_WIDTH * VERTS_HEIGHT * 3);
	allLines = DrawingData((int64_t)VERTS_WIDTH * VERTS_HEIGHT * 6);
	exteriorLines = DrawingData((int64_t)VERTS_WIDTH * VERTS_HEIGHT * 6);
	uniqueExteriorLines = DrawingData((int64_t)VERTS_WIDTH * VERTS_HEIGHT * 6);

	onNodes = DrawingData((int64_t)NODES_WIDTH * NODES_HEIGHT * 3);
	offNodes = DrawingData((int64_t)NODES_WIDTH * NODES_HEIGHT * 3);
}

void Environment::GenerateVertices()
{
	for (int i = 0; i < VERTS_HEIGHT; i++)
	{
		for (int j = 0; j < VERTS_WIDTH; j++)
		{
			*(vertices + ((int64_t)i * VERTS_WIDTH + j) * 3) = (GLfloat)(j) / (VERTS_WIDTH - 1) * 2 - 1;
			*(vertices + ((int64_t)i * VERTS_WIDTH + j) * 3 + 1) = -((GLfloat)(i) / (VERTS_HEIGHT - 1) * 2 - 1);
			*(vertices + ((int64_t)i * VERTS_WIDTH + j) * 3 + 2) = 0;
		}
	}
}

int Environment::GetSurroundingOnNodes(int i, int j)
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

void Environment::SmoothMap()
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
			int count = GetSurroundingOnNodes(i, j);
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

void Environment::GenerateNodeMap()
{
	srand(time(NULL));
	int percentOn = 50;
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
		SmoothMap();
	}
	
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		for (int j = 0; j < NODES_WIDTH; j++)
		{
			if (nodes[i][j] == true) {
				onNodes.AddIndex(2 * (i * VERTS_WIDTH + j));
			} 
			else
			{
				offNodes.AddIndex(2 * (i * VERTS_WIDTH + j));
			}
		}
	}
}

void Environment::MarchAllSquares()
{
	int squareCombs[16][5][3] = {
		{{0}, { 0,0,0 }}, // 0
		{{1}, {0,1,VERTS_WIDTH}}, // 1
		{{1}, {1,2,VERTS_WIDTH + 2}}, // 2
		{{2}, {0,2,VERTS_WIDTH + 2}, {0,VERTS_WIDTH + 2,VERTS_WIDTH}}, // 3
		{{1}, {2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH,VERTS_WIDTH},}, // 4
		{{2}, {0,1,2 * VERTS_WIDTH+1},{0,2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH}}, // 5
		{{4}, {1,2,VERTS_WIDTH+2},{1,VERTS_WIDTH+2,2*VERTS_WIDTH+1},{1,2*VERTS_WIDTH + 1,2 * VERTS_WIDTH},{1,2 * VERTS_WIDTH,VERTS_WIDTH}}, // 6
		{{3}, {0,2,VERTS_WIDTH + 2},{0,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1},{0,2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH}}, // 7
		{{1}, {VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1}}, // 8
		{{4}, {0,1,VERTS_WIDTH + 2},{0,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2},{0,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1},{0,2 * VERTS_WIDTH + 1,VERTS_WIDTH}}, // 9
		{{2}, {1,2,2 * VERTS_WIDTH + 2},{1,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1}}, // 10
		{{3}, {0,2,2*VERTS_WIDTH+2},{0,2 * VERTS_WIDTH + 2,2*VERTS_WIDTH+1},{0,2 * VERTS_WIDTH + 1,VERTS_WIDTH}}, // 11
		{{2}, {VERTS_WIDTH,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2},{VERTS_WIDTH,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH}}, // 12
		{{3}, {0,1,VERTS_WIDTH+2},{0,VERTS_WIDTH + 2,2 * VERTS_WIDTH+2},{0,2 * VERTS_WIDTH+2,2*VERTS_WIDTH}}, // 13
		{{3}, {1,2,2*VERTS_WIDTH + 2},{1,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH},{1,2 * VERTS_WIDTH,VERTS_WIDTH}}, // 14
		{{2}, {0,2,2 * VERTS_WIDTH + 2},{0,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH}}, // 15
	};

	int outLineCombs[16][3][2] = {
		{{0}}, // 0
		{{1}, {1, VERTS_WIDTH}}, // 1
		{{1}, {1, VERTS_WIDTH+2}}, // 2
		{{1}, {VERTS_WIDTH+2,VERTS_WIDTH}}, // 3
		{{1}, {VERTS_WIDTH, 2* VERTS_WIDTH+1}}, // 4
		{{1}, {1,2*VERTS_WIDTH+1}}, // 5
		{{2}, {1, VERTS_WIDTH}, {VERTS_WIDTH+2,2* VERTS_WIDTH+1}}, // 6
		{{1}, {VERTS_WIDTH+2, 2* VERTS_WIDTH+1}}, // 7
		{{1}, {VERTS_WIDTH+2, 2* VERTS_WIDTH+1}}, // 8
		{{2}, {VERTS_WIDTH, 2 * VERTS_WIDTH + 1}, {1, VERTS_WIDTH + 2}}, // 9
		{{1}, {1,2 * VERTS_WIDTH + 1}}, // 10
		{{1}, {VERTS_WIDTH, 2*VERTS_WIDTH + 1}}, // 11
		{{1}, {VERTS_WIDTH + 2,VERTS_WIDTH}}, // 12
		{{1}, {1, VERTS_WIDTH + 2}}, // 13
		{{1}, {1, VERTS_WIDTH}}, // 14
		{{0}}, // 15
	};

	for (int i = 0; i < SQUARES_HEIGHT; i++)
	{
		for (int j = 0; j < SQUARES_WIDTH; j++)
		{
			Square s = squares[(int64_t)i * SQUARES_WIDTH + j];
			s.MarchSquare(nodes, squareCombs, outLineCombs, VERTS_WIDTH);
			if (s.code != 0)
			{
				for (int h = 0; h < s.numTris; h++)
				{
					mainMesh.AddIndex(s.tris[h].v1);
					mainMesh.AddIndex(s.tris[h].v2);
					mainMesh.AddIndex(s.tris[h].v3);

					allLines.AddIndex(s.tris[h].v1);
					allLines.AddIndex(s.tris[h].v2);
					allLines.AddIndex(s.tris[h].v2);
					allLines.AddIndex(s.tris[h].v3);
					allLines.AddIndex(s.tris[h].v3);
					allLines.AddIndex(s.tris[h].v1);
				}

				GLuint* outlineVerts = s.GetOutlineLines();
				for (int g = 0; g < s.numTris + 1; g++)
				{
					exteriorLines.AddIndex(outlineVerts[g]);
					exteriorLines.AddIndex(outlineVerts[g + 1]);
				}
				exteriorLines.AddIndex(outlineVerts[s.numTris + 1]);
				exteriorLines.AddIndex(outlineVerts[0]);

				for (int f = 0; f < s.numOutVerts; f++)
				{
					uniqueExteriorLines.AddIndex(s.outVerts[f]);
				}
			}
		}
	}
}

void Environment::GenerateShaders()
{
	// Generates Vertex Buffer Object and links it to vertices
	VBO.Generate(vertices, (int64_t)VERTS_WIDTH * VERTS_HEIGHT * 3 * sizeof(*vertices));
	
	mainMesh.Generate(VBO, "default.vert", "black.frag", sizeof(*vertices));

	allLines.Generate(VBO, "default.vert", "red.frag", sizeof(*vertices));

	exteriorLines.Generate(VBO, "default.vert", "red.frag", sizeof(*vertices));

	uniqueExteriorLines.Generate(VBO, "default.vert", "red.frag", sizeof(*vertices));

	onNodes.Generate(VBO, "default.vert", "green.frag", sizeof(*vertices));
	offNodes.Generate(VBO, "default.vert", "black.frag", sizeof(*vertices));
}

void Environment::Draw()
{
	mainMesh.shaderProgram.Activate();
	mainMesh.VAO.Bind();
	glDrawElements(GL_TRIANGLES, mainMesh.numVerts, GL_UNSIGNED_INT, 0);

	allLines.shaderProgram.Activate();
	allLines.VAO.Bind();
	//glDrawElements(GL_LINES, allLines.numVerts, GL_UNSIGNED_INT, 0);

	exteriorLines.shaderProgram.Activate();
	exteriorLines.VAO.Bind();
	glDrawElements(GL_LINES, exteriorLines.numVerts, GL_UNSIGNED_INT, 0);

	uniqueExteriorLines.shaderProgram.Activate();
	uniqueExteriorLines.VAO.Bind();
	//glDrawElements(GL_LINES, uniqueExteriorLines.numVerts, GL_UNSIGNED_INT, 0);

	glPointSize(5);
	onNodes.shaderProgram.Activate();
	onNodes.VAO.Bind();
	glDrawElements(GL_POINTS, onNodes.numVerts, GL_UNSIGNED_INT, 0);

	offNodes.shaderProgram.Activate();
	offNodes.VAO.Bind();
	glDrawElements(GL_POINTS, offNodes.numVerts, GL_UNSIGNED_INT, 0);
}

void Environment::ShaderClean()
{
	// Clean up, delete objects created
	VBO.Delete();
	mainMesh.ShaderClean();
	allLines.ShaderClean();
	exteriorLines.ShaderClean();
	uniqueExteriorLines.ShaderClean();
	onNodes.ShaderClean();
	offNodes.ShaderClean();
}

void Environment::Destroy()
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