#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class application
{
public:
	application(int width, int height, const char* title);
	~application();
	void Run();

private:
	GLFWwindow* window;
	unsigned int SCR_WIDTH, SCR_HEIGHT;

	void initGLFW();
	void initGLAD();
	void setupCallbacks();
};

