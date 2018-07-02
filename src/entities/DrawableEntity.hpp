#ifndef PROCEDURALWORLD_DRAWABLEENTITY_HPP
#define PROCEDURALWORLD_DRAWABLEENTITY_HPP

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <vector>
#include "Light.h"
#include "Entity.hpp"

// Abstract class

class DrawableEntity : public Entity {
private:
	GLuint shader_program;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

protected:
	GLenum draw_mode;
	bool should_draw_back_face;
	GLuint initVertexArray(
		const std::vector<glm::vec3>& vertices,
		GLuint* vertices_buffer = nullptr,
		GLuint* element_buffer = nullptr
	);
	GLuint initVertexArray(
		const std::vector<glm::vec3>& vertices,
		const std::vector<GLuint>& elements,
		GLuint* vertices_buffer = nullptr,
		GLuint* element_buffer = nullptr
	);
	GLuint initVertexArray(
        const std::vector<glm::vec3>& vertices,
        const std::vector<GLuint>& elements,
        const std::vector<glm::vec3>& normals,
        GLuint* vertices_buffer = nullptr,
        GLuint* element_buffer = nullptr,
        GLuint* normal_buffer = nullptr
    );
    GLuint initVertexArray(
        const std::vector<glm::vec3>& vertices,
        const std::vector<GLuint>& elements,
        const std::vector<glm::vec3>& normals,
        const std::vector<glm::vec2>& uvs,
        GLuint* vertices_buffer = nullptr,
        GLuint* element_buffer = nullptr,
        GLuint* normal_buffer = nullptr,
        GLuint* uv_buffer = nullptr
    );
    void setMaterial(
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        const float& shininess
    );

public:
	explicit DrawableEntity(const GLuint& shader_program) : DrawableEntity(shader_program, nullptr) {}
	DrawableEntity(const GLuint& shader_program, Entity* parent);
	// pure virtual methods must be overridden by derived classes
	virtual const std::vector<glm::vec3>& getVertices() const = 0;
	virtual GLuint getVAO() = 0;
	virtual const int getColorType() = 0;
	// end pure virtual functions
	virtual GLuint getTextureId();
	GLenum getDrawMode();
	void draw(
		const glm::mat4& view_matrix,
		const glm::mat4& projection_matrix,
		const Light& light
	) override;
};

#endif // PROCEDURALWORLD_DRAWABLEENTITY_HPP
