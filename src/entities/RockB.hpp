#ifndef PROCEDURALWORLD_ROCKB_HPP
#define PROCEDURALWORLD_ROCKB_HPP

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

class RockB : public DrawableEntity {
private:
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> elements;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2>uvs;
    GLuint vao;
    GLuint vertices_buffer;
    GLuint element_buffer;
    GLuint uv_buffer;
    GLuint normal_buffer;
public:
    RockB(
            const GLuint& shader_program,
            const float& world_x_location,
            const float& world_z_location,
            const float& x_span,
            const float& z_span
    ) : RockB(
            shader_program,
            world_x_location,
            world_z_location,
            x_span,
            z_span,
            nullptr
    ) {}
    RockB(
            const GLuint& shader_program,
            const float& world_x_location,
            const float& world_z_location,
            const float& x_span,
            const float& z_span,
            Entity* parent
    );
    ~RockB() override;
    void generateSphere(
            std::vector<glm::vec3>* vertices,
            std::vector<GLuint>* ebo,
            std::vector<glm::vec3>* normals,
            std::vector<glm::vec2>* uvs,
            const int num_arc_segments,
            const int num_longitude_lines);
    const std::vector<glm::vec3>& getVertices() const override;
    GLuint getVAO() override;
    const int getColorType() override;
    GLuint getTextureId() override;
};


#endif //PROCEDURALWORLD_ROCKB_HPP
