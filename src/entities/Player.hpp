#ifndef COMP371_PLAYER_HPP
#define COMP371_PLAYER_HPP

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

class Player: public DrawableEntity {
private:
	static constexpr float base_x_rotation_angle = -1.0f * (float)(M_PI / 2);
	static constexpr float base_y_rotation_angle = (float)M_PI;
	const glm::mat4& getBaseRotation() const override;
	const glm::mat4& getBaseScale() const override;
	const glm::vec3& getDefaultFaceVector() const override;
	std::vector<glm::vec3> vertices;
	GLuint vao;

public:
	explicit Player(const GLuint& shader_program) : Player(shader_program, nullptr) {}
	Player(const GLuint& shader_program, Entity* parent);
	const std::vector<glm::vec3>& getVertices() const override;
	GLuint getVAO() override;
	const int getColorType() override;

	static constexpr int deadMax = 20;
	int deadTime =  deadMax;
	glm::vec3 oldPosition;

	void updateDeadtime();
	bool fore, back, left, right;
};

#endif // COMP371_PLAYER_HPP
