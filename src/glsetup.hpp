#ifndef PACMAN3D_GLSETUP_H
#define PACMAN3D_GLSETUP_H

#define GLFW_INCLUDE_NONE // don't include deprecated gl headers on macOS
#include <GLFW/glfw3.h>	// include GLFW helper library

void setupGlContext(
	const int& width,
	const int& height,
	const char* title,
	GLFWwindow** window
);

#endif //PACMAN3D_GLSETUP_H
