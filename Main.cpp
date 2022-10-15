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
	e.SetUpCombs();
	e.SetDrawingNodes();
	e.MarchAllSquares();


	GLFWwindow* window = GLFWClass::StartUp(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (window == NULL)
	{
		return -1;
	}

	e.GenerateShaders();

	int r_cool = 0;
	int highlight_cool = 0;
	int number_cool = 0;

	// Main while loop until window should close
	while (!glfwWindowShouldClose(window))
	{
		if (r_cool > 0)
		{
			r_cool--;
		}
		if (highlight_cool > 0)
		{
			highlight_cool--;
		}
		if (number_cool > 0)
		{
			number_cool--;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) && r_cool == 0)
		{
			e.nM.SmoothMap();
			e.nM.SetRegionNumbers();
			e.SetDrawingNodes();
			e.MarchAllSquares();

			e.RegenerateShaders();

			r_cool = 1000;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE))
		{
			e.GenerateNodes();
			e.SetDrawingNodes();
			e.MarchAllSquares();

			e.RegenerateShaders();
		}

		if (glfwGetKey(window, GLFW_KEY_UP) && highlight_cool == 0)
		{
			e.RotateSelected(1);
			e.SetDrawingNodes();

			e.RegenerateShaders();

			highlight_cool = 1000;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) && highlight_cool == 0)
		{
			e.RotateSelected(-1);
			e.SetDrawingNodes();

			e.RegenerateShaders();

			highlight_cool = 1000;
		}

		if (glfwGetKey(window, GLFW_KEY_1) && number_cool == 0)
		{
			e.ToggleMesh();
			number_cool = 1000;
		}
		if (glfwGetKey(window, GLFW_KEY_2) && number_cool == 0)
		{
			e.ToggleLines();
			number_cool = 1000;
		}
		if (glfwGetKey(window, GLFW_KEY_3) && number_cool == 0)
		{
			e.ToggleExteriors();
			number_cool = 1000;
		}
		if (glfwGetKey(window, GLFW_KEY_4) && number_cool == 0)
		{
			e.ToggleUniques();
			number_cool = 1000;
		}
		if (glfwGetKey(window, GLFW_KEY_5) && number_cool == 0)
		{
			e.ToggleOns();
			number_cool = 1000;
		}
		if (glfwGetKey(window, GLFW_KEY_6) && number_cool == 0)
		{
			e.ToggleOffs();
			number_cool = 1000;
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