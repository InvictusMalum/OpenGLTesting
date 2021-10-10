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
#include"glfwClass.h"

#include <stdlib.h>
#include <list>

using namespace std;

const int SCREEN_WIDTH = 1080; 
const int SCREEN_HEIGHT = 720; 

const int SQUARES_WIDTH = 30;
const int SQUARES_HEIGHT = 20;

Environment e(SQUARES_WIDTH, SQUARES_HEIGHT);
Player p;

int main() 
{
	p.PopulateRays();
	
	e.GenerateVertices();
	e.GenerateNodeMap();
	e.MarchAllSquares();
	
	
	GLFWwindow* window = GLFWClass::StartUp(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window == NULL)
	{
		return -1;
	}

	e.GenerateShaders();

	// Main while loop until window should close
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT))
		{
			p.Turn(-1);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			p.Turn(1);
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
		{
			std::cout << "Click" << std::endl;
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