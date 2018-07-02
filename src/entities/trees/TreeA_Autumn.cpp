#include <cmath>
#include <src/loadTexture.hpp>

#include "Tree.hpp"
#include "TreeA_Autumn.hpp"

TreeA_Autumn::TreeA_Autumn(const GLuint& shader_program, Entity* entity, float trunkDiameter, const int& seed):
        TreeA(shader_program, entity, trunkDiameter, seed, false){
        textureMap = textureMap2;
};

GLuint TreeA_Autumn::getTextureId()
{
        static GLuint tA_texture = loadTexture(
                TreeA::textureMap,
                GL_NEAREST,
                GL_NEAREST
        );
        return tA_texture;
}
