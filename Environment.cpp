#include<iostream>
#include<set>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stdlib.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include"TrisSquares.h"

#include"Environment.h"

using namespace std;

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

	nM = NodeMap(NODES_WIDTH, NODES_HEIGHT);
	
	vertices = new GLfloat[(int64_t)VERTS_WIDTH * VERTS_HEIGHT*6];

	onNodes = DrawingData((int64_t)NODES_WIDTH * NODES_HEIGHT * 3);
	offNodes = DrawingData((int64_t)NODES_WIDTH * NODES_HEIGHT * 3);

	mainMesh = DrawingData((int64_t)VERTS_WIDTH * VERTS_HEIGHT * 3);
	allLines = DrawingData((int64_t)VERTS_WIDTH * VERTS_HEIGHT * 6);
	exteriorLines = DrawingData((int64_t)VERTS_WIDTH * VERTS_HEIGHT * 6);
	uniqueExteriorLines = DrawingData((int64_t)VERTS_WIDTH * VERTS_HEIGHT * 6);
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

void Environment::GenerateNodes() {
	nM.GenerateNodeMap();
	nM.SetRegionNumbers();
}

void Environment::SetDrawingNodes()
{
	onNodes.Reset();
	offNodes.Reset();
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		for (int j = 0; j < NODES_WIDTH; j++)
		{
			if (nM.nodes[i][j] == 0)
			{
				onNodes.AddIndex(2 * (i * VERTS_WIDTH + j));
			} else if (nM.nodes[i][j] == selectedRegion)
			{
				offNodes.AddIndex(2 * (i * VERTS_WIDTH + j));
			}
		}
	}
}

void Environment::RotateSelected(int n)
{
	if (rotationTimer == 0) {
		rotationTimer = rotationMax;
		selectedRegion += n;
		if (selectedRegion < 2)
		{
			selectedRegion = nM.maxRegionNum;
		}
		if (selectedRegion > nM.maxRegionNum)
		{
			selectedRegion = 2;
		}
	}
}

void Environment::SetUpCombs() 
{
	int squareCombsArray[16][5][3] = {
		{{0}, { 0,0,0 }}, // 0
		{{1}, {0,1,VERTS_WIDTH}}, // 1
		{{1}, {1,2,VERTS_WIDTH + 2}}, // 2
		{{2}, {0,2,VERTS_WIDTH + 2}, {0,VERTS_WIDTH + 2,VERTS_WIDTH}}, // 3
		{{1}, {2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH,VERTS_WIDTH},}, // 4
		{{2}, {0,1,2 * VERTS_WIDTH + 1},{0,2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH}}, // 5
		{{4}, {1,2,VERTS_WIDTH + 2},{1,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1},{1,2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH},{1,2 * VERTS_WIDTH,VERTS_WIDTH}}, // 6
		{{3}, {0,2,VERTS_WIDTH + 2},{0,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1},{0,2 * VERTS_WIDTH + 1,2 * VERTS_WIDTH}}, // 7
		{{1}, {VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1}}, // 8
		{{4}, {0,1,VERTS_WIDTH + 2},{0,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2},{0,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1},{0,2 * VERTS_WIDTH + 1,VERTS_WIDTH}}, // 9
		{{2}, {1,2,2 * VERTS_WIDTH + 2},{1,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1}}, // 10
		{{3}, {0,2,2 * VERTS_WIDTH + 2},{0,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1},{0,2 * VERTS_WIDTH + 1,VERTS_WIDTH}}, // 11
		{{2}, {VERTS_WIDTH,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2},{VERTS_WIDTH,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH}}, // 12
		{{3}, {0,1,VERTS_WIDTH + 2},{0,VERTS_WIDTH + 2,2 * VERTS_WIDTH + 2},{0,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH}}, // 13
		{{3}, {1,2,2 * VERTS_WIDTH + 2},{1,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH},{1,2 * VERTS_WIDTH,VERTS_WIDTH}}, // 14
		{{2}, {0,2,2 * VERTS_WIDTH + 2},{0,2 * VERTS_WIDTH + 2,2 * VERTS_WIDTH}}, // 15
	};

	int outLineCombsArray[16][3][2] = {
		{{0}}, // 0
		{{1}, {1, VERTS_WIDTH}}, // 1
		{{1}, {1, VERTS_WIDTH + 2}}, // 2
		{{1}, {VERTS_WIDTH + 2,VERTS_WIDTH}}, // 3
		{{1}, {VERTS_WIDTH, 2 * VERTS_WIDTH + 1}}, // 4
		{{1}, {1,2 * VERTS_WIDTH + 1}}, // 5
		{{2}, {1, VERTS_WIDTH}, {VERTS_WIDTH + 2,2 * VERTS_WIDTH + 1}}, // 6
		{{1}, {VERTS_WIDTH + 2, 2 * VERTS_WIDTH + 1}}, // 7
		{{1}, {VERTS_WIDTH + 2, 2 * VERTS_WIDTH + 1}}, // 8
		{{2}, {VERTS_WIDTH, 2 * VERTS_WIDTH + 1}, {1, VERTS_WIDTH + 2}}, // 9
		{{1}, {1,2 * VERTS_WIDTH + 1}}, // 10
		{{1}, {VERTS_WIDTH, 2 * VERTS_WIDTH + 1}}, // 11
		{{1}, {VERTS_WIDTH + 2,VERTS_WIDTH}}, // 12
		{{1}, {1, VERTS_WIDTH + 2}}, // 13
		{{1}, {1, VERTS_WIDTH}}, // 14
		{{0}}, // 15
	};

	squareCombs = new int**[16];
	for (int i = 0; i < 16; i++)
	{
		int** newRow = new int*[5];
		for (int j = 0; j < 5; j++)
		{
			int* newRow2 = new int[3];
			for (int k = 0; k < 3; k++)
			{
				newRow2[k] = squareCombsArray[i][j][k];
			}
			newRow[j] = newRow2;
		}
		squareCombs[i] = newRow;
	}

	outLineCombs = new int** [16];
	for (int i = 0; i < 16; i++)
	{
		int** newRow = new int* [3];
		for (int j = 0; j < 5; j++)
		{
			int* newRow2 = new int[2];
			for (int k = 0; k < 3; k++)
			{
				newRow2[k] = outLineCombsArray[i][j][k];
			}
			newRow[j] = newRow2;
		}
		outLineCombs[i] = newRow;
	}

}

void Environment::MarchAllSquares()
{
	mainMesh.Reset();
	allLines.Reset();
	exteriorLines.Reset();
	uniqueExteriorLines.Reset();

	for (int i = 0; i < SQUARES_HEIGHT; i++)
	{
		for (int j = 0; j < SQUARES_WIDTH; j++)
		{
			s = squares[(int64_t)i * SQUARES_WIDTH + j];
			s.MarchSquare(nM.nodes, squareCombs, outLineCombs, VERTS_WIDTH);
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

void Environment::ToggleMesh()
{
	if (meshCountdown == 0)
	{
		renderMesh = !renderMesh;
		meshCountdown = countdownMax;
	}
}

void Environment::ToggleLines()
{
	if (linesCountdown == 0)
	{
		renderLines = !renderLines;
		linesCountdown = countdownMax;
	}
}

void Environment::ToggleExteriors()
{
	if (exteriorsCountdown == 0)
	{
		renderExteriors = !renderExteriors;
		exteriorsCountdown = countdownMax;
	}
}

void Environment::ToggleUniques()
{
	if (uniquesCountdown == 0)
	{
		renderUniques = !renderUniques;
		uniquesCountdown = countdownMax;
	}
}

void Environment::ToggleOns()
{
	if (onsCountdown == 0)
	{
		renderOns = !renderOns;
		onsCountdown = countdownMax;
	}
}

void Environment::ToggleOffs()
{
	if (offsCountdown == 0)
	{
		renderOffs = !renderOffs;
		offsCountdown = countdownMax;
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

	VBO.Unbind();
	VBO.Delete();
}

void Environment::Draw()
{
	if (renderMesh)
	{
		mainMesh.shaderProgram.Activate();
		mainMesh.VAO.Bind();
		glDrawElements(GL_TRIANGLES, mainMesh.numVerts, GL_UNSIGNED_INT, 0);
	}

	if (renderLines)
	{
		allLines.shaderProgram.Activate();
		allLines.VAO.Bind();
		glDrawElements(GL_LINES, allLines.numVerts, GL_UNSIGNED_INT, 0);
	}

	if (renderExteriors)
	{
		exteriorLines.shaderProgram.Activate();
		exteriorLines.VAO.Bind();
		glDrawElements(GL_LINES, exteriorLines.numVerts, GL_UNSIGNED_INT, 0);
	}

	if (renderUniques)
	{
		uniqueExteriorLines.shaderProgram.Activate();
		uniqueExteriorLines.VAO.Bind();
		glDrawElements(GL_LINES, uniqueExteriorLines.numVerts, GL_UNSIGNED_INT, 0);
	}

	glPointSize(5);
	if (renderOns)
	{
		onNodes.shaderProgram.Activate();
		onNodes.VAO.Bind();
		glDrawElements(GL_POINTS, onNodes.numVerts, GL_UNSIGNED_INT, 0);
	}

	if (renderOffs)
	{
		offNodes.shaderProgram.Activate();
		offNodes.VAO.Bind();
		glDrawElements(GL_POINTS, offNodes.numVerts, GL_UNSIGNED_INT, 0);
	}

	if (meshCountdown > 0)
	{
		meshCountdown--;
	}

	if (linesCountdown > 0)
	{
		linesCountdown--;
	}

	if (exteriorsCountdown > 0)
	{
		exteriorsCountdown--;
	}

	if (uniquesCountdown > 0)
	{
		uniquesCountdown--;
	}

	if (onsCountdown > 0)
	{
		onsCountdown--;
	}

	if (offsCountdown > 0)
	{
		offsCountdown--;
	}

	if (rotationTimer > 0)
	{
		rotationTimer--;
	}
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