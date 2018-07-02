#ifndef PACMAN3D_ENTITY_H
#define PACMAN3D_ENTITY_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include "Light.h"
#include <glm/glm.hpp>
#include <vector>

class Entity {
private:
	Entity* parent;
	std::vector<Entity*> children;
	glm::mat4 scale_matrix;
	glm::mat4 rotation_matrix;
    glm::mat4 translation_matrix;
	glm::mat4 model_matrix;
	float opacity;
	bool hidden;
	void orient(const glm::vec3& forward_vec);

protected:
	virtual const glm::mat4& getBaseScale() const;
	virtual const glm::mat4& getBaseRotation() const;
	virtual const glm::mat4& getBaseTranslation() const;
	virtual const glm::vec3& getDefaultFaceVector() const;
	// this method is NOT responsible for freeing memory
	// and does NOT remove the parent pointer from the child
	void detachChild(Entity* const& child);

public:
	Entity() : Entity(nullptr) {}
	explicit Entity(Entity* parent);
	virtual ~Entity() = default;
	const glm::mat4 getModelMatrix() const;
	glm::vec3 getPosition() const;
	glm::vec3 getScale() const;
	float getOpacity();
	bool isHidden();
	void scale(const float& scalar);
    void scale(const glm::vec3& scale_vector);
    void rotate(const float& angle, const glm::vec3& axis);
    void resetRotation();
    void moveForward(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
    void moveBack(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
    void moveLeft(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
    void moveRight(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void moveForwardLeft(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void moveForwardRight(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void moveBackLeft(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
	void moveBackRight(const glm::vec3& view_vec, const glm::vec3& up_vec, const float& units = 1.0f);
    void translate(const glm::vec3& translation_vec);
	void setPosition(const glm::vec3& position);
	void setOpacity(const float& opacity);
	void hide();
	void unhide();
	void toggleHide();
	virtual void draw(
		const glm::mat4& view_matrix,
		const glm::mat4& projection_matrix,
		const Light& light
	);
};

#endif //PACMAN3D_ENTITY_H
