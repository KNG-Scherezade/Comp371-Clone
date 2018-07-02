////////DISCONTINUED////////////

#ifndef COMP371_TREE_C_HPP
#define COMP371_TREE_C_HPP

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include <iostream>
#include <cstdio>
#include <ctime>


#include "src/entities/DrawableEntity.hpp"
#include "Tree.hpp"
#include "TreeClusterItem.hpp"
#include "src/TreeRandom.hpp"

class TreeC{
private:
    std::vector<Tree*>* trees;

    static constexpr float sizeVariation = 1.2;
    static constexpr int heightChunking = 0;
    static constexpr int boostFactor = 0;
    static int spacingConstant;
    int maxWidth(const float& trunkDiameter);

public:

    TreeC(int numberOfTrees, const GLuint& shader_program,
          Entity* entity, float trunkDiameter, long seed, bool isAlien,
            std::vector<Tree*>& treeContainer, glm::vec3 pos, float magnitude,
          float min_hitbox_y, float max_hitbox_y, std::vector<HitBox2d>& hbEnt);

    static void setSpacingConstant(int k);

};
#endif