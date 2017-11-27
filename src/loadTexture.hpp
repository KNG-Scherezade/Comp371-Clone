#ifndef PROCEDURALWORLD_LOADTEXTURE_HPP
#define PROCEDURALWORLD_LOADTEXTURE_HPP

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <string>

GLuint loadTexture(
	const std::string& path,
	const GLint& min_filter,
	const GLint& mag_filter
);

GLuint loadTexture(
		const std::string& path,
		const GLint& min_filter,
		const GLint& mag_filter,
		const GLint& paramS,
		const GLint& paramT
);

#endif //PROCEDURALWORLD_LOADTEXTURE_HPP
