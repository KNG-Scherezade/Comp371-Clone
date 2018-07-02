
#ifndef PROCEDURALWORLD_LEAFB_H
#define PROCEDURALWORLD_LEAFB_H

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "src/TreeRandom.hpp"
#include <cstdio>
class LeafBranchAB {
    int randomSeedValue;
    float itterations;
    int branchPoints;
    float trunkDiameter;
    float lineHeight;
    float jagednessFactor_Leaf;
    std::vector<glm::vec3> *leafVertices;
    std::vector<GLuint> *leafIndices;
    std::vector<glm::vec2> *leafUVs;
public:
    float getLineHeight();

    LeafBranchAB(const int& randomSeedValue, const float& itterations, const int& branchPoints,
                const float& lineHeight, const float& jagednessFactor_Leaf,
                std::vector<glm::vec3>* leafVertices,
                std::vector<GLuint>* leafIndices, std::vector<glm::vec2>* leafUVs);

    void buildLeafBranch(const float& trunkDiameter, int y, float textureLeafStart);

    void buildBranchElements(int baseVerticesSize, int i, int len);
};
#endif //leafBranch