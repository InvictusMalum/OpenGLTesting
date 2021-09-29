#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
using namespace std;

GLint SCREEN_WIDTH = 800;
GLint SCREEN_HEIGHT = 800;

const int vSize = ((SCREEN_WIDTH + 1) * (SCREEN_HEIGHT + 1) * 3);

GLfloat* vertices[] = new GLfloat*[vSize];

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};

int main() 
{

	for (int i = -1; i <= 1.0f; i += 2.0f/SCREEN_HEIGHT)
	{
		for (int j = -1; j <= 1.0f; j += 2.0f/SCREEN_WIDTH)
		{
			
			vertices[(i * SCREEN_HEIGHT + j) * 3] = i;
			vertices[(i * SCREEN_HEIGHT + j) * 3 + 1] = j;
			vertices[(i * SCREEN_HEIGHT + j) * 3 + 2] = 0;
		}
	}

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
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("default.vert", "default.frag");

	//GLfloat value = -1.0f;

	// Main while loop until window should close
	while (!glfwWindowShouldClose(window))
	{
		// Generates Vertex Array Object and binds it
		VAO VAO1;
		VAO1.Bind();


		//vertices[0] = value;

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


		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL the Shader Program we want to use
		shaderProgram.Activate();
		// Bind VAO so OpenGL knows how to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Swap back and front buffers
		glfwSwapBuffers(window);
		// Get and process events
		glfwPollEvents();


		//value += 0.0001f;
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