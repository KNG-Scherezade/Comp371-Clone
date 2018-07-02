#ifndef PROCEDURALWORLD_SKYBOX_H
#define PROCEDURALWORLD_SKYBOX_H

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

class Skybox: public DrawableEntity {
private:
	std::vector<glm::vec3> verticies;
public:

	explicit Skybox(const GLuint& shader_program) : Skybox(shader_program, nullptr) {}
	Skybox(const GLuint& shader_program, Entity* parent);

	~Skybox() override;

	const std::vector<glm::vec3>& getVertices() const override;
	GLuint getVAO() override;
	const int getColorType() override;
    GLuint getTextureId() override;
};

#endif //PROCEDURALWORLD_SKYBOX_H
