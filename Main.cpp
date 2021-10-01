#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

#include <stdlib.h>
#include <list>

using namespace std;

const GLint SCREEN_WIDTH = 800; 
const GLint SCREEN_HEIGHT = 800; 

const GLint SQUARES_WIDTH = 300;
const GLint SQUARES_HEIGHT = 300;

const GLint NODES_WIDTH = SQUARES_WIDTH+1;
const GLint NODES_HEIGHT = SQUARES_HEIGHT+1;

const GLint VERTS_WIDTH = 2 * SQUARES_WIDTH + 1;
const GLint VERTS_HEIGHT = 2 * SQUARES_HEIGHT + 1;

const int vSize = (VERTS_WIDTH * VERTS_HEIGHT * 3);

GLfloat vertices[vSize];
bool nodes[NODES_HEIGHT][NODES_WIDTH];
bool oldNodes[NODES_HEIGHT][NODES_WIDTH];
GLuint nodeIndices[SQUARES_HEIGHT * SQUARES_WIDTH * 6];
GLuint indices[VERTS_HEIGHT * VERTS_WIDTH * 3];

int x = VERTS_WIDTH;

int squareCombs[16][5][3] = {
	{{0}, { 0,0,0 }}, // 0
	{{1}, {0,1,x}}, // 1
	{{1}, {1,2,x + 2}}, // 2
	{{2}, {0,x + 2,x},{0,2,x + 2}}, // 3
	{{1}, {x,2 * x + 1,2 * x}}, // 4
	{{2}, {0,1,2 * x},{1,2 * x + 1,2 * x}}, // 5
	{{4}, {1,2,x},{2,2 * x,x},{2,x + 2,2 * x},{x + 2,2 * x + 1,2 * x}}, // 6
	{{3}, {0,2,x + 2},{0,x + 2,2 * x + 1},{0,2 * x + 1,2 * x}}, // 7
	{{1}, {x+2,2*x+2,2*x+1}}, // 8
	{{4}, {0,1,x+2},{0,x+2,2*x+2},{0,2*x+2,2*x+1},{0,2*x+1,x}}, // 9
	{{2}, {1,2,2*x+1},{2,2*x+1,2*x+2}}, // 10
	{{3}, {0,2,x},{2,2*x+1,x},{2,2*x+2,2*x+1}}, // 11
	{{2}, {x,x+2,2*x+2},{x,2*x+2,2*x}}, // 12
	{{3}, {0,1,2*x},{1,x+2,2*x},{x+2,2*x+2,2*x}}, // 13
	{{3}, {1,2,2*x+2},{1,2*x+2,x},{x,2*x+2,2*x}}, // 14
	{{2}, {0,2,2*x+2},{0,2*x+2,2*x}}, // 15
};

int getSurroundingOnNodes(int i, int j)
{
	int count = 0;
	for (int neighborI = i - 1; neighborI <= i + 1; neighborI++)
	{
		for (int neighborJ = j - 1; neighborJ <= j + 1; neighborJ++)
		{
			if (neighborI != i || neighborJ != j)
			{
				if (neighborI >= 0 && neighborI < NODES_WIDTH && neighborJ >= 0 && neighborJ < NODES_HEIGHT)
				{
					if (oldNodes[neighborI][neighborJ])
					{
						count++;
					}
				}
				else
				{
					count++;
				}
			}
		}
	}
	return count;
};

void smoothMap()
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
			} 
			else if (count < 4)
			{
				nodes[i][j] = false;
			};
		};
	};
};



int main() 
{
	// Make all vertices
	for (int i = 0; i < VERTS_HEIGHT; i++)
	{
		for (int j = 0; j < VERTS_WIDTH; j++)
		{
			vertices[(i * VERTS_WIDTH + j) * 3] = (GLfloat)(j)/(VERTS_WIDTH-1)*2-1;
			vertices[(i * VERTS_WIDTH + j) * 3 + 1] = -((GLfloat)(i) / (VERTS_HEIGHT-1)*2-1);
			vertices[(i * VERTS_WIDTH + j) * 3 + 2] = 0;
		}
	}

	// Generate Node Map
	srand(time(NULL));
	int percentOn = 50;
	int outLayer = 0;
	for (int i = 0; i < NODES_HEIGHT; i++)
	{
		for (int j = 0; j < NODES_WIDTH; j++)
		{
			if (i == 0 || j == 0 || i == NODES_HEIGHT-1 || j == NODES_WIDTH -1 || rand() % 100 + 1 <= percentOn)
			{
				nodes[i][j] = true;
			}
			else
			{
				nodes[i][j] = false;
			}
		}
	}
	smoothMap();
	smoothMap();
	smoothMap();



	int nextOpen = 0;
	// Marching Squares
	uint8_t code = 0;
	for (int i = 0; i < SQUARES_HEIGHT; i++)
	{
		for (int j = 0; j < SQUARES_WIDTH; j++)
		{
			code = 0;
			if (nodes[i][j])
				code += 1;
			if (nodes[i][j+1])
				code += 2;
			if (nodes[i+1][j])
				code += 4;
			if (nodes[i+1][j+1])
				code += 8;

			GLuint startVert = 2 * (i * VERTS_WIDTH + j);
			if (code != 0)
			{
				for (int tri = 1; tri <= squareCombs[code][0][0]; tri++)
				{
					indices[nextOpen] = startVert + squareCombs[code][tri][0];
					indices[nextOpen+1] = startVert + squareCombs[code][tri][1];
					indices[nextOpen+2] = startVert + squareCombs[code][tri][2];
					nextOpen += 3;
				}
			}
		}
	}

	std::cout << "Made " << nextOpen << " verts" << std::endl;

	// Initialize GLFW
	glfwInit();


	// Use Hints to tell GLFW what version of OpenGL we are using
	// OpenGL 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFW window, width SCREEN_WIDTH, height SCREEN_HEIGHT, named "OpenGLTest"
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGLTest", NULL, NULL);
	// If window fails to be created
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Make window be in current context
	glfwMakeContextCurrent(window);

	// Load GLAD to configure OpenGL
	gladLoadGL();

	// Tell GLAD the viewport size of OpenGL in the window
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader shaderProgram("default.vert", "default.frag");

	// Main while loop until window should close
	while (!glfwWindowShouldClose(window))
	{
		// Generates Vertex Array Object and binds it
		VAO VAO1;
		VAO1.Bind();

		// Generates Vertex Buffer Object and links it to vertices
		VBO VBO1(vertices, sizeof(vertices));
		// Generates Element Buffer Object and links it to indices
		EBO EBO1(indices, sizeof(indices));

		// Links VBO to VAO
		VAO1.LinkVBO(VBO1, 0);
		// Unbind all to prevent accidentally modifying them
		VAO1.Unbind();
		VBO1.Unbind();
		EBO1.Unbind();


		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL the Shader Program we want to use
		shaderProgram.Activate();
		// Bind VAO so OpenGL knows how to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, nextOpen, GL_UNSIGNED_INT, 0);
		// Swap back and front buffers
		glfwSwapBuffers(window);
		// Get and process events
		glfwPollEvents();

		VAO1.Delete();
		VBO1.Delete();
		EBO1.Delete();
	}

	// Clean up, delete objects created
	//VAO1.Delete();
	//VBO1.Delete();
	//EBO1.Delete();
	shaderProgram.Delete();

	// End program
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}