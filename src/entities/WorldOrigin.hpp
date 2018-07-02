#ifndef PACMAN3D_WORLDORIGIN_H
#define PACMAN3D_WORLDORIGIN_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <vector>

#include "Entity.hpp"
#include "DrawableEntity.hpp"

class WorldOrigin: public DrawableEntity {
private:
	std::vector<glm::vec3> vertices;
	GLuint vao;
public:
	explicit WorldOrigin(const GLuint& shader_program) : WorldOrigin(shader_program, nullptr) {}
	WorldOrigin(const GLuint& shader_program, Entity* parent)
		: WorldOrigin(shader_program, 10, 10, 10, parent) {}
	WorldOrigin(
		const GLuint& shader_program,
		const int& x_max,
		const int& y_max,
		const int& z_max
	) : WorldOrigin(shader_program, x_max, y_max, z_max, nullptr) {}
	WorldOrigin(
		const GLuint& shader_program,
		const int& x_max,
		const int& y_max,
		const int& z_max,
		Entity* parent
	);
	const std::vector<glm::vec3>& getVertices() const override;
	GLuint getVAO() override;
	const int getColorType() override;
};

#endif //PACMAN3D_WORLDORIGIN_H
