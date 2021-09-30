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

const GLint VERTS_WIDTH = 800;
const GLint VERTS_HEIGHT = 800;

const int vSize = ((SCREEN_WIDTH + 1) * (SCREEN_HEIGHT + 1) * 3);

GLfloat vertices[vSize];

// Indices for vertices order
GLuint indices[VERTS_HEIGHT * VERTS_WIDTH * 6];

int main() 
{
	for (int i = 0; i < VERTS_HEIGHT; i++)
	{
		for (int j = 0; j < VERTS_WIDTH; j++)
		{
			vertices[(i * VERTS_WIDTH + j) * 3] = (GLfloat)(j)/(VERTS_WIDTH-1)*2-1;
			vertices[(i * VERTS_WIDTH + j) * 3 + 1] = -((GLfloat)(i) / (VERTS_HEIGHT-1)*2-1);
			vertices[(i * VERTS_WIDTH + j) * 3 + 2] = 0;
		}
	}
	srand(time(NULL));

	int percentOn = 50;

	int nextOpen = 0;
	for (int i = 0; i < VERTS_HEIGHT-1; i++)
	{
		for (int j = 0; j < VERTS_WIDTH-1; j++)
		{
			if (rand() % 100 + 1 <= percentOn)
			{
				indices[nextOpen] = i * VERTS_WIDTH + j;
				indices[nextOpen + 1] = i * VERTS_WIDTH + j + VERTS_WIDTH + 1;
				indices[nextOpen + 2] = i * VERTS_WIDTH + j + VERTS_WIDTH;
				
				indices[nextOpen+3] = i * VERTS_WIDTH + j;
				indices[nextOpen+4] = i * VERTS_WIDTH + j + 1;
				indices[nextOpen+5] = i * VERTS_WIDTH + j + VERTS_WIDTH + 1;
				nextOpen += 6;
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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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