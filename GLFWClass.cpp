#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"GLFWClass.h"

GLFWwindow* GLFWClass::StartUp(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
	// Initialize GLFW
	glfwInit();
	// Use Hints to tell GLFW what version of OpenGL is being used
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
		return NULL;
	}
	// Make window be in current context
	glfwMakeContextCurrent(window);
	// Load GLAD to configure OpenGL
	gladLoadGL();
	// Tell GLAD the viewport size of OpenGL in the window
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	return window;
}