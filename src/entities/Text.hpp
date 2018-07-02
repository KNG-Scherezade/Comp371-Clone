#ifndef PROCEDURALWORLD_TEXT_HPP
#define PROCEDURALWORLD_TEXT_HPP



#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <string>
#include "Entity.hpp"
#include "DrawableEntity.hpp"

class Text : public DrawableEntity {
private:
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> elements;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2>uvs;
    GLuint font_map;
    GLuint vao;
    GLuint vertices_buffer;
    GLuint element_buffer;
    GLuint uv_buffer;
    GLuint normal_buffer;
public:
    Text(
            const GLuint& shader_program,
            const std::string& message,
            const int line_height,
            const float left_margin,
            const unsigned int FONT_STYLE
    ) : Text(
            shader_program,
            message,
            line_height,
            left_margin,
            FONT_STYLE,
            nullptr
    ){}
    Text(
            const GLuint& shader_program,
            const std::string& message,
            const int line_height,
            const float left_margin,
            const unsigned int FONT_STYLE,
            Entity* parent
    );
    ~Text() override;
    const std::vector<glm::vec3>& getVertices() const override;
    GLuint getVAO() override;
    const int getColorType() override;
    GLuint getTextureId() override;

};



#endif //PROCEDURALWORLD_TEXT_HPP
