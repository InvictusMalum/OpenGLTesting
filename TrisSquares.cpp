#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"TrisSquares.h"

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

void Square::MarchSquare(int** nodes, int*** squareCombs, int*** outLineCombs, int VERTS_WIDTH)
{
	GLuint j = c1 / 2 % VERTS_WIDTH;
	GLuint i = (c1 / 2 - j) / VERTS_WIDTH;

	code = 0;
	if (nodes[i][j] == 0)
		code += 1;
	if (nodes[i][j + 1] == 0)
		code += 2;
	if (nodes[i + 1][j] == 0)
		code += 4;
	if (nodes[i + 1][j + 1] == 0)
		code += 8;

	GLuint startVert = c1;
	if (code != 0)
	{
		numTris = *(squareCombs + code)[0][0];
		for (int tri = 1; tri <= numTris; tri++)
		{
			tris[tri - 1].SetVerts(c1 + squareCombs[code][tri][0], c1 + squareCombs[code][tri][1], c1 + squareCombs[code][tri][2]);
		}

		if (code != 15)
		{
			int numLines = outLineCombs[code][0][0];
			numOutVerts = numLines * 2;

			int nextVert = 0;
			for (int i = 1; i <= numLines; i++)
			{
				outVerts[nextVert] = c1 + outLineCombs[code][i][0];
				outVerts[nextVert + 1] = c1 + outLineCombs[code][i][1];
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
		out[2 + i] = tris[i].v3;
	}
	return out;
}
