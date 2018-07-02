#ifndef PACMAN3D_SHADER_PROGRAM_H
#define PACMAN3D_SHADER_PROGRAM_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <string>

GLuint prepareShaderProgram(
	const std::string& vertex_shader_path,
	const std::string& fragment_shader_path,
	bool* ok
);

#endif //PACMAN3D_SHADER_PROGRAM_H
