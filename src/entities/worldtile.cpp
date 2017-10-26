#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "entity.hpp"
#include "drawableentity.hpp"
#include "worldtile.hpp"

#include "../constants.hpp"

WorldTile::WorldTile(const GLuint& shader_program, Entity* parent) : DrawableEntity(shader_program, parent)
{
    this->draw_mode = GL_TRIANGLES;

    int tile_width=1, tile_height = 1;

    //tile at the origin 1x1 in XZ plane
    this->vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    this->vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    this->vertices.push_back(glm::vec3(1.0f, 0.0f, -1.0f));
    this->vertices.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

    std::vector<GLuint> elements;
    WorldTile::createElements(tile_width, tile_height, &elements);
    this->vao = this->initVertexArray(
            this->vertices,
            elements,
            &this->vertices_buffer,
            &this->element_buffer
    );



    //this->vao = this->initVertexArray(this->vertices);
}

const std::vector<glm::vec3>& WorldTile::getVertices()
{
    return this->vertices;
}

GLuint WorldTile::getVAO()
{
    return this->vao;
}

const int WorldTile::getColorType()
{
    return COLOR_TILE;
}

const glm::mat4& WorldTile::getBaseRotation()
{
    static glm::vec3 x_axis = glm::vec3(1.0f, 0.0f, 0.0f);
    static glm::mat4 identity;
    static glm::mat4 rotation = glm::rotate(identity, WorldTile::base_rotation_angle, x_axis);

    return rotation;
}

void WorldTile::createElements(
        const int& width,
        const int& height,
        std::vector<GLuint>* const elements
) {
    if (elements == nullptr) {
        throw std::runtime_error("Elements vector must not be null.");
    }

    // since we know our terrain comes from an image, i.e. a fully-filled 2d array,
    // we can create an element array using an uncomplicated algorithm which creates
    // two triangles to connect each arrangement of 4 adjacent vertices.

            // first triangle
            elements->emplace_back(3); // top-left
            elements->emplace_back(0); // bottom-left
            elements->emplace_back(2); // top-right

            // second triangle
            elements->emplace_back(0); // bottom-left
            elements->emplace_back(1); // bottom-right
            elements->emplace_back(2); // top-right
}

// the vector indicating the direction the model faces by default (with no rotation)
const glm::vec3& WorldTile::getDefaultFaceVector()
{
    static glm::vec3 default_face_vec(1.0f, 0.0f, 0.0f);

    return default_face_vec;
}


