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

const int SCREEN_WIDTH = 1080; 
const int SCREEN_HEIGHT = 720; 

const int SQUARES_WIDTH = 150;
const int SQUARES_HEIGHT = 100;

Environment e(SQUARES_WIDTH, SQUARES_HEIGHT);
Player p;

int main() 
{
	p.populateRays();
	
	e.GenerateVertices();
	e.GenerateNodeMap();
	e.MarchSquares();
	
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

	e.GenerateShaders();

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

		e.Draw();

		// Update Screen
		// Swap back and front buffers
		glfwSwapBuffers(window);
		// Get and process events
		glfwPollEvents();

	}
	e.ShaderClean();
	e.Destroy();

	// End program
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}