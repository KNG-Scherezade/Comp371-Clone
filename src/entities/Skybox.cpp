#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>

#include "Entity.hpp"
#include "DrawableEntity.hpp"

#include "Skybox.hpp"
#include "../utils.hpp"
#include "../constants.hpp"
#include "../loadTexture.hpp"
#include "../objloader.hpp"


float size = 10;

Skybox::Skybox(const GLuint& shader_program, Entity* parent) : DrawableEntity(shader_program, parent)
{
    this->draw_mode = GL_TRIANGLES;

    this->setMaterial(
            glm::vec3(.5, .5, .5), // need to change this to some other value... maybe the height of the plane if we ever make it.
            glm::vec3(.5, .5, .5),
            glm::vec3(.25, .25, .25),
            25.0f
    );
}

Skybox::~Skybox() {}

const std::vector<glm::vec3>& Skybox::getVertices() const {
	/*
	static const std::vector<glm::vec3> vertices = {
			//front square
            glm::vec3(-size,-size, size), //front down left point   0
            glm::vec3( size,-size, size), //front down right point  1
            glm::vec3(-size, size, size), //front top left point    2
            glm::vec3( size, size, size), //front top right point   3
            //right square
            glm::vec3( size,-size, size), //left down left point    4
            glm::vec3( size,-size,-size), //left down right point   5
            glm::vec3( size, size, size), //left top left point     6
            glm::vec3( size, size,-size), //left top right point    7
            //back square
            glm::vec3(-size,-size, -size), //back down left point   8
            glm::vec3( size,-size, -size), //back down right point  9
            glm::vec3(-size, size, -size), //back top left point    10
            glm::vec3( size, size, -size), //back top right point   11
            //left square
            glm::vec3(-size,-size,-size), //left down left point    12
            glm::vec3(-size,-size, size), //left down right point   13
            glm::vec3(-size, size,-size), //left top left point     14
            glm::vec3(-size, size, size), //left top right point    15
            //up square
            glm::vec3(-size, size, size), //up down left point      16
            glm::vec3( size, size, size), //up down right point     17
            glm::vec3(-size, size,-size), //up top left point       18
            glm::vec3( size, size,-size), //up top right point      19
            //down square
            glm::vec3(-size,-size,-size), //down down left point    20
            glm::vec3( size,-size,-size), //down down right point   21
            glm::vec3(-size,-size, size), //down top left point     22
            glm::vec3( size,-size, size), //down top right point    23
	};
*/
	return this->verticies;
}

GLuint Skybox::getVAO() {

	std::vector<GLuint> elements;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	loadOBJ("../models/domeshpere.obj", &this->verticies, &normals, &uvs, &elements);


	/*
	static const std::vector<GLuint> elements {
            //front square
            2, 1, 0,
            3, 1, 2,
            //right square
            6, 5, 4,
            7, 5, 6,
            //back square
            10, 9, 8,
            11, 9, 10,
            //left square
            14, 13, 12,
            15, 13, 14,
            //up square
            18, 17, 16,
            19, 17, 18,
            //down square
            22, 21, 20,
            23, 25, 22
	};

	static const std::vector<glm::vec3> normals {
            //front
            glm::vec3(0,0,-1),
            glm::vec3(0,0,-1),
            glm::vec3(0,0,-1),
            glm::vec3(0,0,-1),
            //right
            glm::vec3(-1,0,0),
            glm::vec3(-1,0,0),
            glm::vec3(-1,0,0),
            glm::vec3(-1,0,0),
            //back
            glm::vec3(0,0,1),
            glm::vec3(0,0,1),
            glm::vec3(0,0,1),
            glm::vec3(0,0,1),
            //left
            glm::vec3(1,0,0),
            glm::vec3(1,0,0),
            glm::vec3(1,0,0),
            glm::vec3(1,0,0),
            //top
            glm::vec3(0,-1,0),
            glm::vec3(0,-1,0),
            glm::vec3(0,-1,0),
            glm::vec3(0,-1,0),
            //down
            glm::vec3(0,1,0),
            glm::vec3(0,1,0),
            glm::vec3(0,1,0),
            glm::vec3(0,1,0)
	};

    static const std::vector<glm::vec2> uvs {
        //front view
        glm::vec2 (0, 1/3),
        glm::vec2 (1/4, 2/3),
        glm::vec2 (0, 2/3),
        glm::vec2 (1/4, 1/3),
        //right view
        glm::vec2(1/4, 1/3),
        glm::vec2(2/4, 1/3),
        glm::vec2(1/4, 2/3),
        glm::vec2(2/4, 2/3),
        //back view
        glm::vec2(2/4, 1/3),
        glm::vec2(3/4, 1/4),
        glm::vec2(2/3, 2/3),
        glm::vec2(3/4, 2/3),
        //left view
        glm::vec2(3/4, 1/3),
        glm::vec2(1, 1/3),
        glm::vec2(3/4, 2/3),
        glm::vec2(1, 2/3),
        //top view
        glm::vec2(1/4, 2/3),
        glm::vec2(2/4, 2/3),
        glm::vec2(1/4, 1),
        glm::vec2(2/4, 1),
        //bottom view
        glm::vec2(1/4, 0),
        glm::vec2(2/4, 0),
        glm::vec2(1/4, 1/3),
        glm::vec2(2/4, 1/3)
    };
	*/
	static GLuint vao;
	static bool vao_init = false;

	if (!vao_init) {
		// only initialize vao once for all instances
		vao = this->initVertexArray(
				this->getVertices(),
				elements,
				normals,
                uvs
		);
		vao_init = true;
	}

	return vao;
}

const int Skybox::getColorType() {
    return COLOR_SKY_TEXTURE;
}

GLuint Skybox::getTextureId()
{
    //from pinterest
    static GLuint cloud_texture = loadTexture(
            "../textures/nightskydome1.jpg",
            GL_LINEAR,
            GL_LINEAR
    );
    return cloud_texture;
}