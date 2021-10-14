#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include <stdlib.h>

#include"DrawingData.h"

DrawingData::DrawingData(int64_t indicesLength_)
{
	indicesLength = indicesLength_;
	indices = new GLuint[indicesLength];
}

void DrawingData::Reset()
{
	for (int i = 0; i < indicesLength; i++) {
		indices[i] = 0;
	}
	numVerts = 0;
}

void DrawingData::AddIndex(GLuint num)
{
	indices[numVerts] = num;
	numVerts++;
}

void DrawingData::Generate(VBO VBO, const char* vertShader, const char* fragShader, int sizeMult)
{
	shaderProgram.Generate(vertShader, fragShader);
	VAO.Generate();
	VAO.Bind();
	EBO.Generate(indices, (int64_t)numVerts * sizeMult);
	VAO.LinkVBO(VBO, 0);

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void DrawingData::ShaderClean()
{
	VAO.Delete();
	EBO.Delete();
	shaderProgram.Delete();
}