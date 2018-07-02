#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <stdexcept>

#include "Entity.hpp"
#include "DrawableEntity.hpp"
#include "Rock.hpp"

#include "../constants.hpp"
#include "../loadTexture.hpp"
#include "../utils.hpp"


Rock::Rock(
    const GLuint &shader_program,
    const float &world_x_location,
    const float &world_z_location,
    const float &x_span,
    const float &z_span,
    Entity *parent
) : DrawableEntity(shader_program, parent)
{
    static const std::vector<GLuint> elements = {
        24, 19, 18, 23, 24, 18, 23, 18, 17, 22, 23, 17, 22, 17, 16, 21, 22, 16, 21, 16, 15, 20, 21, 15, 19, 14, 13, 18, 19, 13, 18, 13, 12, 17, 18, 12, 17, 12, 11, 16, 17, 11, 16, 11, 10, 15, 16, 10, 14, 9, 8, 13, 14, 8, 13, 8, 7, 12, 13, 7, 12, 7, 6, 11, 12, 6, 11, 6, 5, 10, 11, 5, 9, 4, 3, 8, 9, 3, 8, 3, 2, 7, 8, 2, 7, 2, 1, 6, 7, 1, 6, 1, 0, 5, 6, 0,

        49, 44, 43, 48, 49, 43, 48, 43, 42, 47, 48, 42, 47, 42, 41, 46, 47, 41, 46, 41, 40, 45, 46, 40, 44, 39, 38, 43, 44, 38, 43, 38, 37, 42, 43, 37, 42, 37, 36, 41, 42, 36, 41, 36, 35, 40, 41, 35, 39, 34, 33, 38, 39, 33, 38, 33, 32, 37, 38, 32, 37, 32, 31, 36, 37, 31, 36, 31, 30, 35, 36, 30, 34, 20, 15, 33, 34, 15, 33, 15, 10, 32, 33, 10, 32, 10, 5, 31, 32, 5, 31, 5, 0, 30, 31, 0,

        74, 69, 68, 73, 74, 68, 73, 68, 67, 72, 73, 67, 72, 67, 66, 71, 72, 66, 71, 66, 44, 49, 71, 44, 69, 64, 63, 68, 69, 63, 68, 63, 62, 67, 68, 62, 67, 62, 61, 66, 67, 61, 66, 61, 39, 44, 66, 39, 64, 59, 58, 63, 64, 58, 63, 58, 57, 62, 63, 57, 62, 57, 56, 61, 62, 56, 61, 56, 34, 39, 61, 34, 59, 24, 23, 58, 59, 23, 58, 23, 22, 57, 58, 22, 57, 22, 21, 56, 57, 21, 56, 21, 20, 34, 56, 20,

        4, 81, 80, 4, 9, 81, 9, 82, 81, 9, 14, 82, 14, 83, 82, 14, 19, 83, 19, 59, 83, 19, 24, 59, 80, 86, 85, 80, 81, 86, 81, 87, 86, 81, 82, 87, 82, 88, 87, 82, 83, 88, 83, 64, 88, 83, 59, 64, 85, 91, 90, 85, 86, 91, 86, 92, 91, 86, 87, 92, 87, 93, 92, 87, 88, 93, 88, 69, 93, 88, 64, 69, 90, 96, 95, 90, 91, 96, 91, 97, 96, 91, 92, 97, 92, 98, 97, 92, 93, 98, 93, 74, 98, 93, 69, 74,

        0, 106, 30, 0, 1, 106, 1, 107, 106, 1, 2, 107, 2, 108, 107, 2, 3, 108, 3, 80, 108, 3, 4, 80, 30, 111, 35, 30, 106, 111, 106, 112, 111, 106, 107, 112, 107, 113, 112, 107, 108, 113, 108, 85, 113, 108, 80, 85, 35, 116, 40, 35, 111, 116, 111, 117, 116, 111, 112, 117, 112, 118, 117, 112, 113, 118, 113, 90, 118, 113, 85, 90, 40, 121, 45, 40, 116, 121, 116, 122, 121, 116, 117, 122, 117, 123, 122, 117, 118, 123, 118, 95, 123, 118, 90, 95,


        45, 131, 46, 45, 121, 131, 121, 132, 131, 121, 122, 132, 122, 133, 132, 122, 123, 133, 123, 96, 133, 123, 95, 96, 46, 136, 47, 46, 131, 136, 131, 137, 136, 131, 132, 137, 132, 138, 137, 132, 133, 138, 133, 97, 138, 133, 96, 97, 47, 141, 48, 47, 136, 141, 136, 142, 141, 136, 137, 142, 137, 143, 142, 137, 138, 143, 138, 98, 143, 138, 97, 98, 48, 71, 49, 48, 141, 71, 141, 72, 71, 141, 142, 72, 142, 73, 72, 142, 143, 73, 143, 74, 73, 143, 98, 74
    };

    static const std::vector<glm::vec2> uvs = {
            glm::vec2(0.00f, 0.00f),
            glm::vec2(0.25f, 0.00f),
            glm::vec2(0.50f, 0.00f),
            glm::vec2(0.75f, 0.00f),
            glm::vec2(1.00f, 0.00f),

            glm::vec2(0.00f, 0.25f),
            glm::vec2(0.25f, 0.25f),
            glm::vec2(0.50f, 0.25f),
            glm::vec2(0.75f, 0.25f),
            glm::vec2(1.00f, 0.25f),

            glm::vec2(0.00f, 0.50f),
            glm::vec2(0.25f, 0.50f),
            glm::vec2(0.50f, 0.50f),
            glm::vec2(0.75f, 0.50f),
            glm::vec2(1.00f, 0.50f),

            glm::vec2(0.00f, 0.75f),
            glm::vec2(0.25f, 0.75f),
            glm::vec2(0.50f, 0.75f),
            glm::vec2(0.75f, 0.75f),
            glm::vec2(1.00f, 0.75f),

            glm::vec2(0.00f, 1.00f),
            glm::vec2(0.25f, 1.00f),
            glm::vec2(0.50f, 1.00f),
            glm::vec2(0.75f, 1.00f),
            glm::vec2(1.00f, 1.00f),

            //left side face

            glm::vec2(0.00f, 0.00f),
            glm::vec2(0.25f, 0.00f),
            glm::vec2(0.50f, 0.00f),
            glm::vec2(0.75f, 0.00f),
            glm::vec2(1.00f, 0.00f),

            glm::vec2(0.00f, 0.25f),
            glm::vec2(0.25f, 0.25f),
            glm::vec2(0.50f, 0.25f),
            glm::vec2(0.75f, 0.25f),
            glm::vec2(1.00f, 0.25f),

            glm::vec2(0.00f, 0.50f),
            glm::vec2(0.25f, 0.50f),
            glm::vec2(0.50f, 0.50f),
            glm::vec2(0.75f, 0.50f),
            glm::vec2(1.00f, 0.50f),

            glm::vec2(0.00f, 0.75f),
            glm::vec2(0.25f, 0.75f),
            glm::vec2(0.50f, 0.75f),
            glm::vec2(0.75f, 0.75f),
            glm::vec2(1.00f, 0.75f),

            glm::vec2(0.00f, 1.00f),
            glm::vec2(0.25f, 1.00f),
            glm::vec2(0.50f, 1.00f),
            glm::vec2(0.75f, 1.00f),
            glm::vec2(1.00f, 1.00f),

            //back face

            glm::vec2(0.00f, 0.00f),
            glm::vec2(0.25f, 0.00f),
            glm::vec2(0.50f, 0.00f),
            glm::vec2(0.75f, 0.00f),
            glm::vec2(1.00f, 0.00f),

            glm::vec2(0.00f, 0.25f),
            glm::vec2(0.25f, 0.25f),
            glm::vec2(0.50f, 0.25f),
            glm::vec2(0.75f, 0.25f),
            glm::vec2(1.00f, 0.25f),

            glm::vec2(0.00f, 0.50f),
            glm::vec2(0.25f, 0.50f),
            glm::vec2(0.50f, 0.50f),
            glm::vec2(0.75f, 0.50f),
            glm::vec2(1.00f, 0.50f),

            glm::vec2(0.00f, 0.75f),
            glm::vec2(0.25f, 0.75f),
            glm::vec2(0.50f, 0.75f),
            glm::vec2(0.75f, 0.75f),
            glm::vec2(1.00f, 0.75f),

            glm::vec2(0.00f, 1.00f),
            glm::vec2(0.25f, 1.00f),
            glm::vec2(0.50f, 1.00f),
            glm::vec2(0.75f, 1.00f),
            glm::vec2(1.00f, 1.00f),

            //right side face

            glm::vec2(0.00f, 0.00f),
            glm::vec2(0.25f, 0.00f),
            glm::vec2(0.50f, 0.00f),
            glm::vec2(0.75f, 0.00f),
            glm::vec2(1.00f, 0.00f),

            glm::vec2(0.00f, 0.25f),
            glm::vec2(0.25f, 0.25f),
            glm::vec2(0.50f, 0.25f),
            glm::vec2(0.75f, 0.25f),
            glm::vec2(1.00f, 0.25f),

            glm::vec2(0.00f, 0.50f),
            glm::vec2(0.25f, 0.50f),
            glm::vec2(0.50f, 0.50f),
            glm::vec2(0.75f, 0.50f),
            glm::vec2(1.00f, 0.50f),

            glm::vec2(0.00f, 0.75f),
            glm::vec2(0.25f, 0.75f),
            glm::vec2(0.50f, 0.75f),
            glm::vec2(0.75f, 0.75f),
            glm::vec2(1.00f, 0.75f),

            glm::vec2(0.00f, 1.00f),
            glm::vec2(0.25f, 1.00f),
            glm::vec2(0.50f, 1.00f),
            glm::vec2(0.75f, 1.00f),
            glm::vec2(1.00f, 1.00f),

            //front face
            glm::vec2(0.00f, 0.00f),
            glm::vec2(0.25f, 0.00f),
            glm::vec2(0.50f, 0.00f),
            glm::vec2(0.75f, 0.00f),
            glm::vec2(1.00f, 0.00f),

            glm::vec2(0.00f, 0.25f),
            glm::vec2(0.25f, 0.25f),
            glm::vec2(0.50f, 0.25f),
            glm::vec2(0.75f, 0.25f),
            glm::vec2(1.00f, 0.25f),

            glm::vec2(0.00f, 0.50f),
            glm::vec2(0.25f, 0.50f),
            glm::vec2(0.50f, 0.50f),
            glm::vec2(0.75f, 0.50f),
            glm::vec2(1.00f, 0.50f),

            glm::vec2(0.00f, 0.75f),
            glm::vec2(0.25f, 0.75f),
            glm::vec2(0.50f, 0.75f),
            glm::vec2(0.75f, 0.75f),
            glm::vec2(1.00f, 0.75f),

            glm::vec2(0.00f, 1.00f),
            glm::vec2(0.25f, 1.00f),
            glm::vec2(0.50f, 1.00f),
            glm::vec2(0.75f, 1.00f),
            glm::vec2(1.00f, 1.00f),

            //top face
            glm::vec2(0.00f, 0.00f),
            glm::vec2(0.25f, 0.00f),
            glm::vec2(0.50f, 0.00f),
            glm::vec2(0.75f, 0.00f),
            glm::vec2(1.00f, 0.00f),

            glm::vec2(0.00f, 0.25f),
            glm::vec2(0.25f, 0.25f),
            glm::vec2(0.50f, 0.25f),
            glm::vec2(0.75f, 0.25f),
            glm::vec2(1.00f, 0.25f),

            glm::vec2(0.00f, 0.50f),
            glm::vec2(0.25f, 0.50f),
            glm::vec2(0.50f, 0.50f),
            glm::vec2(0.75f, 0.50f),
            glm::vec2(1.00f, 0.50f),

            glm::vec2(0.00f, 0.75f),
            glm::vec2(0.25f, 0.75f),
            glm::vec2(0.50f, 0.75f),
            glm::vec2(0.75f, 0.75f),
            glm::vec2(1.00f, 0.75f),

            glm::vec2(0.00f, 1.00f),
            glm::vec2(0.25f, 1.00f),
            glm::vec2(0.50f, 1.00f),
            glm::vec2(0.75f, 1.00f),
            glm::vec2(1.00f, 1.00f)
    };

    this->draw_mode = GL_TRIANGLES;

    // modified obsidian material from
    // Advanced Graphics Programming Using OpenGL
    // by Tom McReynolds and David Blythe
    // and copied from slides from COMP371 at Concordia:
    // http://poullis.org/courses/2017/Fall/COMP371/resources/COMP371F17_LightingandShading.pdf

    float ambientR =  utils::randomFloat(0.02f, 0.7f);
    float ambientG = utils::randomFloat(0.02f, 0.07f);
    float ambientB = utils::randomFloat(0.05f, 0.1f);

    float diffuseR = utils::randomFloat(0.65f, 0.75f);
    float diffuseG = utils::randomFloat(0.65f, 0.75f);
    float diffuseB = utils::randomFloat(0.7f, 0.85f);

    float shininess = utils::randomFloat(38.4f, 42.8f);

	this->setMaterial(
		glm::vec3(ambientR, ambientG, ambientB),
		glm::vec3(diffuseR, diffuseG, diffuseB),
		glm::vec3(0.332741, 0.328634, 0.346435),
		shininess
	);

    //bottom face (5x5 between 0 - 1)
    this->vertices.emplace_back(0.00f, 0.00f, 0.00f);
    this->vertices.emplace_back(0.25f, 0.00f, 0.00f);
    this->vertices.emplace_back(0.50f, 0.00f, 0.00f);
    this->vertices.emplace_back(0.75f, 0.00f, 0.00f);
    this->vertices.emplace_back(1.00f, 0.00f, 0.00f);

    this->vertices.emplace_back(0.00f, 0.00f, -0.25f);
    this->vertices.emplace_back(0.25f, 0.00f, -0.25f);
    this->vertices.emplace_back(0.50f, 0.00f, -0.25f);
    this->vertices.emplace_back(0.75f, 0.00f, -0.25f);
    this->vertices.emplace_back(1.00f, 0.00f, -0.25f);

    this->vertices.emplace_back(0.00f, 0.00f, -0.50f);
    this->vertices.emplace_back(0.25f, 0.00f, -0.50f);
    this->vertices.emplace_back(0.50f, 0.00f, -0.50f);
    this->vertices.emplace_back(0.75f, 0.00f, -0.50f);
    this->vertices.emplace_back(1.00f, 0.00f, -0.50f);

    this->vertices.emplace_back(0.00f, 0.00f, -0.75f);
    this->vertices.emplace_back(0.25f, 0.00f, -0.75f);
    this->vertices.emplace_back(0.50f, 0.00f, -0.75f);
    this->vertices.emplace_back(0.75f, 0.00f, -0.75f);
    this->vertices.emplace_back(1.00f, 0.00f, -0.75f);

    this->vertices.emplace_back(0.00f, 0.00f, -1.0f);
    this->vertices.emplace_back(0.25f, 0.00f, -1.0f);
    this->vertices.emplace_back(0.50f, 0.00f, -1.0f);
    this->vertices.emplace_back(0.75f, 0.00f, -1.0f);
    this->vertices.emplace_back(1.00f, 0.00f, -1.0f);

    //left (side) face
    this->vertices.emplace_back(0.00f, 0.00f, 0.00f);
    this->vertices.emplace_back(0.00f, 0.00f, -0.25f);
    this->vertices.emplace_back(0.00f, 0.00f, -0.50f);
    this->vertices.emplace_back(0.00f, 0.00f, -0.75f);
    this->vertices.emplace_back(0.00f, 0.00f, -1.00f);

    this->vertices.emplace_back(0.00f, 0.25f, 0.00f);
    this->vertices.emplace_back(0.00f, 0.25f, -0.25f);
    this->vertices.emplace_back(0.00f, 0.25f, -0.50f);
    this->vertices.emplace_back(0.00f, 0.25f, -0.75f);
    this->vertices.emplace_back(0.00f, 0.25f, -1.00f);

    this->vertices.emplace_back(0.00f, 0.50f, 0.00f);
    this->vertices.emplace_back(0.00f, 0.50f, -0.25f);
    this->vertices.emplace_back(0.00f, 0.50f, -0.50f);
    this->vertices.emplace_back(0.00f, 0.50f, -0.75f);
    this->vertices.emplace_back(0.00f, 0.50f, -1.00f);

    this->vertices.emplace_back(0.00f, 0.75f, 0.00f);
    this->vertices.emplace_back(0.00f, 0.75f, -0.25f);
    this->vertices.emplace_back(0.00f, 0.75f, -0.50f);
    this->vertices.emplace_back(0.00f, 0.75f, -0.75f);
    this->vertices.emplace_back(0.00f, 0.75f, -1.00f);

    this->vertices.emplace_back(0.00f, 1.00f, 0.00f);
    this->vertices.emplace_back(0.00f, 1.00f, -0.25f);
    this->vertices.emplace_back(0.00f, 1.00f, -0.50f);
    this->vertices.emplace_back(0.00f, 1.00f, -0.75f);
    this->vertices.emplace_back(0.00f, 1.00f, -1.00f);


    //back face (5x5 between 0 - 1)
    this->vertices.emplace_back(0.00f, 0.00f, -1.00f);
    this->vertices.emplace_back(0.25f, 0.00f, -1.00f);
    this->vertices.emplace_back(0.50f, 0.00f, -1.00f);
    this->vertices.emplace_back(0.75f, 0.00f, -1.00f);
    this->vertices.emplace_back(1.00f, 0.00f, -1.00f);

    this->vertices.emplace_back(0.00f, 0.25f, -1.00f);
    this->vertices.emplace_back(0.25f, 0.25f, -1.00f);
    this->vertices.emplace_back(0.50f, 0.25f, -1.00f);
    this->vertices.emplace_back(0.75f, 0.25f, -1.00f);
    this->vertices.emplace_back(1.00f, 0.25f, -1.00f);

    this->vertices.emplace_back(0.00f, 0.50f, -1.00f);
    this->vertices.emplace_back(0.25f, 0.50f, -1.00f);
    this->vertices.emplace_back(0.50f, 0.50f, -1.00f);
    this->vertices.emplace_back(0.75f, 0.50f, -1.00f);
    this->vertices.emplace_back(1.00f, 0.50f, -1.00f);

    this->vertices.emplace_back(0.00f, 0.75f, -1.00f);
    this->vertices.emplace_back(0.25f, 0.75f, -1.00f);
    this->vertices.emplace_back(0.50f, 0.75f, -1.00f);
    this->vertices.emplace_back(0.75f, 0.75f, -1.00f);
    this->vertices.emplace_back(1.00f, 0.75f, -1.00f);

    this->vertices.emplace_back(0.00f, 1.00f, -1.0f);
    this->vertices.emplace_back(0.25f, 1.00f, -1.0f);
    this->vertices.emplace_back(0.50f, 1.00f, -1.0f);
    this->vertices.emplace_back(0.75f, 1.00f, -1.0f);
    this->vertices.emplace_back(1.00f, 1.00f, -1.0f);

    //right (side) face
    this->vertices.emplace_back(1.00f, 0.00f, 0.00f);
    this->vertices.emplace_back(1.00f, 0.00f, -0.25f);
    this->vertices.emplace_back(1.00f, 0.00f, -0.50f);
    this->vertices.emplace_back(1.00f, 0.00f, -0.75f);
    this->vertices.emplace_back(1.00f, 0.00f, -1.00f);

    this->vertices.emplace_back(1.00f, 0.25f, 0.00f);
    this->vertices.emplace_back(1.00f, 0.25f, -0.25f);
    this->vertices.emplace_back(1.00f, 0.25f, -0.50f);
    this->vertices.emplace_back(1.00f, 0.25f, -0.75f);
    this->vertices.emplace_back(1.00f, 0.25f, -1.00f);

    this->vertices.emplace_back(1.00f, 0.50f, 0.00f);
    this->vertices.emplace_back(1.00f, 0.50f, -0.25f);
    this->vertices.emplace_back(1.00f, 0.50f, -0.50f);
    this->vertices.emplace_back(1.00f, 0.50f, -0.75f);
    this->vertices.emplace_back(1.00f, 0.50f, -1.00f);

    this->vertices.emplace_back(1.00f, 0.75f, 0.00f);
    this->vertices.emplace_back(1.00f, 0.75f, -0.25f);
    this->vertices.emplace_back(1.00f, 0.75f, -0.50f);
    this->vertices.emplace_back(1.00f, 0.75f, -0.75f);
    this->vertices.emplace_back(1.00f, 0.75f, -1.00f);

    this->vertices.emplace_back(1.00f, 1.00f, 0.00f);
    this->vertices.emplace_back(1.00f, 1.00f, -0.25f);
    this->vertices.emplace_back(1.00f, 1.00f, -0.50f);
    this->vertices.emplace_back(1.00f, 1.00f, -0.75f);
    this->vertices.emplace_back(1.00f, 1.00f, -1.00f);

    //front face (5x5 between 0 - 1)
    this->vertices.emplace_back(0.00f, 0.00f, 0.00f);
    this->vertices.emplace_back(0.25f, 0.00f, 0.00f);
    this->vertices.emplace_back(0.50f, 0.00f, 0.00f);
    this->vertices.emplace_back(0.75f, 0.00f, 0.00f);
    this->vertices.emplace_back(1.00f, 0.00f, 0.00f);

    this->vertices.emplace_back(0.00f, 0.25f, 0.00f);
    this->vertices.emplace_back(0.25f, 0.25f, 0.00f);
    this->vertices.emplace_back(0.50f, 0.25f, 0.00f);
    this->vertices.emplace_back(0.75f, 0.25f, 0.00f);
    this->vertices.emplace_back(1.00f, 0.25f, 0.00f);

    this->vertices.emplace_back(0.00f, 0.50f, 0.00f);
    this->vertices.emplace_back(0.25f, 0.50f, 0.00f);
    this->vertices.emplace_back(0.50f, 0.50f, 0.00f);
    this->vertices.emplace_back(0.75f, 0.50f, 0.00f);
    this->vertices.emplace_back(1.00f, 0.50f, 0.00f);

    this->vertices.emplace_back(0.00f, 0.75f, 0.00f);
    this->vertices.emplace_back(0.25f, 0.75f, 0.00f);
    this->vertices.emplace_back(0.50f, 0.75f, 0.00f);
    this->vertices.emplace_back(0.75f, 0.75f, 0.00f);
    this->vertices.emplace_back(1.00f, 0.75f, 0.00f);

    this->vertices.emplace_back(0.00f, 1.00f, 0.0f);
    this->vertices.emplace_back(0.25f, 1.00f, 0.0f);
    this->vertices.emplace_back(0.50f, 1.00f, 0.0f);
    this->vertices.emplace_back(0.75f, 1.00f, 0.0f);
    this->vertices.emplace_back(1.00f, 1.00f, 0.0f);

    //top face (5x5 between 0 - 1)
    this->vertices.emplace_back(0.00f, 1.00f, 0.00f);
    this->vertices.emplace_back(0.25f, 1.00f, 0.00f);
    this->vertices.emplace_back(0.50f, 1.00f, 0.00f);
    this->vertices.emplace_back(0.75f, 1.00f, 0.00f);
    this->vertices.emplace_back(1.00f, 1.00f, 0.00f);

    this->vertices.emplace_back(0.00f, 1.00f, -0.25f);
    this->vertices.emplace_back(0.25f, 1.00f, -0.25f);
    this->vertices.emplace_back(0.50f, 1.00f, -0.25f);
    this->vertices.emplace_back(0.75f, 1.00f, -0.25f);
    this->vertices.emplace_back(1.00f, 1.00f, -0.25f);

    this->vertices.emplace_back(0.00f, 1.00f, -0.50f);
    this->vertices.emplace_back(0.25f, 1.00f, -0.50f);
    this->vertices.emplace_back(0.50f, 1.00f, -0.50f);
    this->vertices.emplace_back(0.75f, 1.00f, -0.50f);
    this->vertices.emplace_back(1.00f, 1.00f, -0.50f);

    this->vertices.emplace_back(0.00f, 1.00f, -0.75f);
    this->vertices.emplace_back(0.25f, 1.00f, -0.75f);
    this->vertices.emplace_back(0.50f, 1.00f, -0.75f);
    this->vertices.emplace_back(0.75f, 1.00f, -0.75f);
    this->vertices.emplace_back(1.00f, 1.00f, -0.75f);

    this->vertices.emplace_back(0.00f, 1.00f, -1.0f);
    this->vertices.emplace_back(0.25f, 1.00f, -1.0f);
    this->vertices.emplace_back(0.50f, 1.00f, -1.0f);
    this->vertices.emplace_back(0.75f, 1.00f, -1.0f);
    this->vertices.emplace_back(1.00f, 1.00f, -1.0f);

    for (int i=0; i<this->vertices.size(); i++){

        //random number between 0-1 , multiplied by 25 (the space between vertices is 0.25, - 13 (to have it go around zero
        // divided be 100 to put it back to 0.xx
        float jitterX = ((((float) rand())/(RAND_MAX))*25 - 13)/100;
        float jitterY = ((((float) rand())/(RAND_MAX))*25 - 13)/100;
        float jitterZ = ((((float) rand())/(RAND_MAX))*25 - 13)/100;

        this->vertices[i] = this->vertices[i] + glm::vec3(jitterX, jitterY, jitterZ);

    }

    // calculate normals
    //------------------

    //step 0: fill normals with a unit normal so the vector is the right size and hidden vertices will still have a default normal.

    for(int i=0; i<this->vertices.size(); i++){
        this->normals.emplace_back(1.0f,1.0f,1.0f);
    }

    //step 1: find the surface normal for each triangle in the element buffer

    std::vector<glm::vec3>surfaceNormals;

    for(int i=3; i<elements.size()+1; i++){
        if(i%3 == 0){// then element at i-1 is the third vertex of the triangle
            // vertex A is at i-3, vertex B is i-2, vertex C is i-1
            glm::vec3 line_seg_BA = this->vertices[elements[i-2]] - this->vertices[elements[i-3]];
            glm::vec3 line_seg_BC = this->vertices[elements[i-2]] - this->vertices[elements[i-1]];
            //  this is right or this cube, but if the normals look inverted in another application, just switch the order of the cross product operation: cross(line_seg_BA, line_seg_BC)
            glm::vec3 normal = glm::cross(line_seg_BC, line_seg_BA);
            surfaceNormals.emplace_back(normal);
        }
    }

    //step 2: find the average of the surface normals of the surfaces this vertex is part of

    std::vector<glm::vec3>connectedSurfaces;

    for(int i=0; i<this->vertices.size(); i++){
        // vector that will hold all normals of all the surfaces this vertex is part of
        connectedSurfaces.clear();
        for(int j=0; j<elements.size(); j++){
            if(i==elements[j]){//the vertice is being drawn
                //j/3 gives us the location of this surface normal
                connectedSurfaces.emplace_back(surfaceNormals[j/3]);
            }
        }

        if(!connectedSurfaces.empty()){
            glm::vec3 sum = glm::vec3(0.0f);
            for(int k=0; k<connectedSurfaces.size(); k++){
                sum += connectedSurfaces[k];
            }
            //average the normal for this vertex and update the normals buffer
            this->normals[i] = glm::normalize(
                    glm::vec3(sum.x/connectedSurfaces.size(),
                              sum.y/connectedSurfaces.size(),
                              sum.z/connectedSurfaces.size()
                    )
            );
        }
    }

    this->vao = DrawableEntity::initVertexArray(
            this->vertices,
            elements,
            this->normals,
            uvs,
            &this->vertices_buffer,
            &this->element_buffer,
            &this->normal_buffer,
            &this->uv_buffer
    );
}

Rock::~Rock()
{
    glDeleteBuffers(1, &this->vertices_buffer);
    glDeleteBuffers(1, &this->element_buffer);
    glDeleteBuffers(1, &this->uv_buffer);
    glDeleteBuffers(1, &this->uv_buffer);
    glDeleteVertexArrays(1, &this->vao);
}

const std::vector<glm::vec3>& Rock::getVertices() const
{
    return this->vertices;
}

GLuint Rock::getVAO()
{
    return this->vao;
}

const int Rock::getColorType()
{
    return COLOR_LIGHTING;
}

GLuint Rock::getTextureId()
{
	static GLuint rock_texture = loadTexture(
			"../textures/stoneC.jpg",
			GL_NEAREST,
			GL_NEAREST
	);
	return rock_texture;
}
