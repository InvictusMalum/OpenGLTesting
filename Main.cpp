#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Vector2.h"
#include"Player.h"
#include"Environment.h"

#include <stdlib.h>
#include <list>

using namespace std;

const int SCREEN_WIDTH = 1000; 
const int SCREEN_HEIGHT = 1000; 

const int SQUARES_WIDTH = 50;
const int SQUARES_HEIGHT = 50;

GLfloat vertices[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower Left
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower Right
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Top
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner Left
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner Right
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner Down
};

GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};

int main() 
{
	Player p;
	p.populateRays();
	
	Environment e(SQUARES_WIDTH, SQUARES_HEIGHT);
	e.generateVertices();
	e.generateNodeMap();
	e.marchSquares();
	
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

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	
	VBO VBO1(e.vertices, sizeof(e.vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(e.indices, sizeof(e.indices));
	// Links VBO to VAO
	VAO1.LinkVBO(VBO1, 0);
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Main while loop until window should close
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_SPACE))
		{
			std::cout << "SPACE" << std::endl;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
		{
			std::cout << "CLICK" << std::endl;
		}

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Tell OpenGL the Shader Program we want to use
		shaderProgram.Activate();
		// Bind VAO so OpenGL knows how to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, e.numVertices, GL_UNSIGNED_INT, 0);

		// Update Screen
		// Swap back and front buffers
		glfwSwapBuffers(window);
		// Get and process events
		glfwPollEvents();

	}

	// Clean up, delete objects created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	e.destroy();

	// End program
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}