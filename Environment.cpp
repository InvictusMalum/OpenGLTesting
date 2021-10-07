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

	VERTS_WIDTH = 2 * SQUARES_WIDTH + 1;
	VERTS_HEIGHT = 2 * SQUARES_HEIGHT + 1;

	squares = new Square[(int64_t)SQUARES_WIDTH * SQUARES_HEIGHT];
	for (int i = 0; i < SQUARES_HEIGHT; i++)
	{
		for (int j = 0; j < SQUARES_WIDTH; j++)
		{
			squares[(int64_t)i * SQUARES_WIDTH + j].SetCorners(2*(i * VERTS_WIDTH + j), 2*(i * SQUARES_WIDTH + j) + 2*VERTS_WIDTH + 2);
		}
	}
	vertices = new GLfloat[(int64_t)VERTS_WIDTH * VERTS_HEIGHT * 3];
	
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
	
	//nodeIndices = new GLuint[(int64_t)SQUARES_HEIGHT * SQUARES_WIDTH * 6];
	
	indices = new GLuint[(int64_t)VERTS_HEIGHT * VERTS_WIDTH * 3];
	lineIndices = new GLuint[(int64_t)VERTS_HEIGHT * VERTS_WIDTH * 6];
	extLineIndices = new GLuint[(int64_t)VERTS_HEIGHT * VERTS_WIDTH * 6];
	unqExtLineIndices = new GLuint[(int64_t)VERTS_HEIGHT * VERTS_WIDTH * 6];
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

	int nextOpen = 0;
	int nextLineOpen = 0;
	int nextExtLineOpen = 0;
	int nextUnqExtLineOpen = 0;
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
					indices[nextOpen] = s.tris[h].v1;
					indices[nextOpen + 1] = s.tris[h].v2;
					indices[nextOpen + 2] = s.tris[h].v3;
					nextOpen += 3;

					lineIndices[nextLineOpen] = s.tris[h].v1;
					lineIndices[nextLineOpen + 1] = s.tris[h].v2;
					lineIndices[nextLineOpen + 2] = s.tris[h].v2;
					lineIndices[nextLineOpen + 3] = s.tris[h].v3;
					lineIndices[nextLineOpen + 4] = s.tris[h].v3;
					lineIndices[nextLineOpen + 5] = s.tris[h].v1;
					nextLineOpen += 6;
				}

				GLuint* outlineVerts = s.GetOutlineLines();
				for (int g = 0; g < s.numTris + 1; g++)
				{
					extLineIndices[nextExtLineOpen] = outlineVerts[g];
					extLineIndices[nextExtLineOpen + 1] = outlineVerts[g + 1];
					nextExtLineOpen += 2;
				}
				extLineIndices[nextExtLineOpen] = outlineVerts[s.numTris + 1];
				extLineIndices[nextExtLineOpen + 1] = outlineVerts[0];
				nextExtLineOpen += 2;

				for (int f = 0; f < s.numOutVerts; f++)
				{
					unqExtLineIndices[nextUnqExtLineOpen] = s.outVerts[f];
					nextUnqExtLineOpen++;
				}
			}
		}
	}
	numMeshVerts = nextOpen;
	numLineVerts = nextLineOpen;
	numExtLineVerts = nextExtLineOpen;
	numUnqExtLineVerts = nextUnqExtLineOpen;
}

void Environment::GenerateShaders()
{
	shaderProgram.Generate("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO1.Generate();
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO1.Generate(vertices, (int64_t)VERTS_WIDTH * VERTS_HEIGHT * 3 * sizeof(*vertices));
	// Generates Element Buffer Object and links it to indices
	EBO1.Generate(indices, numMeshVerts * sizeof(*vertices));
	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);
	// Unbind all to prevent accidentally modifying them
	
	shaderProgram2.Generate("default.vert", "red.frag");
	VAO2.Generate();
	VAO2.Bind();
	EBO2.Generate(lineIndices, numLineVerts * sizeof(*vertices));
	VAO2.LinkVBO(VBO1, 0);

	shaderProgram3.Generate("default.vert", "red.frag");
	VAO3.Generate();
	VAO3.Bind();
	EBO3.Generate(extLineIndices, (numExtLineVerts) * sizeof(*vertices));
	VAO3.LinkVBO(VBO1, 0);

	shaderProgram4.Generate("default.vert", "red.frag");
	VAO4.Generate();
	VAO4.Bind();
	EBO4.Generate(unqExtLineIndices, numUnqExtLineVerts * sizeof(*vertices));
	VAO4.LinkVBO(VBO1, 0);


	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	
	VAO2.Unbind();
	EBO2.Unbind();

	VAO3.Unbind();
	EBO3.Unbind();

	VAO4.Unbind();
	EBO4.Unbind();
}

void Environment::Draw()
{
	// Tell OpenGL the Shader Program to use
	shaderProgram.Activate();
	// Bind VAO so OpenGL knows how to use it
	VAO1.Bind();
	// Draw the triangle using the GL_TRIANGLES primitive
	glDrawElements(GL_TRIANGLES, numMeshVerts, GL_UNSIGNED_INT, 0);

	shaderProgram2.Activate();
	VAO2.Bind();
	//glDrawElements(GL_LINES, numLineVerts, GL_UNSIGNED_INT, 0);

	shaderProgram3.Activate();
	VAO3.Bind();
	//glDrawElements(GL_LINES, numExtLineVerts, GL_UNSIGNED_INT, 0);

	shaderProgram4.Activate();
	VAO4.Bind();
	glLineWidth(3);
	glEnable(GL_LINE_SMOOTH);
	glDrawElements(GL_LINES, numUnqExtLineVerts, GL_UNSIGNED_INT, 0);
}

void Environment::ShaderClean()
{
	// Clean up, delete objects created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
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