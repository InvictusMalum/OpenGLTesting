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

const int SQUARES_WIDTH = 150;
const int SQUARES_HEIGHT = 100;

Environment e(SQUARES_WIDTH, SQUARES_HEIGHT);
Player p;

int main() 
{
	p.PopulateRays();
	
	e.GenerateVertices();
	e.GenerateNodes();
	e.SetDrawingNodes();
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
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			e.nM.SmoothMap();
			e.nM.SetRegionNumbers();
			e.SetDrawingNodes();
			e.MarchAllSquares();
			
			e.ShaderClean();
			e.GenerateShaders();
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE))
		{
			e.GenerateNodes();
			e.SetDrawingNodes();
			e.MarchAllSquares();
			
			e.ShaderClean();
			e.GenerateShaders();
		}

		if (glfwGetKey(window, GLFW_KEY_UP))
		{
			e.RotateSelected(1);
			e.SetDrawingNodes();
			
			e.ShaderClean();
			e.GenerateShaders();
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
			e.RotateSelected(-1);
			e.SetDrawingNodes();
			
			e.ShaderClean();
			e.GenerateShaders();
		}

		if (glfwGetKey(window, GLFW_KEY_1))
		{
			e.ToggleMesh();
		}
		if (glfwGetKey(window, GLFW_KEY_2))
		{
			e.ToggleLines();
		}
		if (glfwGetKey(window, GLFW_KEY_3))
		{
			e.ToggleExteriors();
		}
		if (glfwGetKey(window, GLFW_KEY_4))
		{
			e.ToggleUniques();
		}
		if (glfwGetKey(window, GLFW_KEY_5))
		{
			e.ToggleOns();
		}
		if (glfwGetKey(window, GLFW_KEY_6))
		{
			e.ToggleOffs();
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