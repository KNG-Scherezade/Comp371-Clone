//
// Created by Tof on 2017-11-19.
//

#ifndef PROCEDURALWORLD_LEAFCONTAINER_H
#define PROCEDURALWORLD_LEAFCONTAINER_H

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "LeafAB.hpp"
#include "LeafBranchAB.hpp"
#include "src/entities/Trees/TrunkAB.hpp"
#include "src/TreeRandom.hpp"


class LeafContainerAB {
public:
    static constexpr int leafPoints = 4;
    static constexpr int leafBranchPoints = 4;
    static constexpr float itterationsLeaf = 360.0f / leafPoints;
    static constexpr float itterationsLeafBranch = 360.0f / leafBranchPoints;

    static constexpr float jagednessFactor_Leaf = 0.35;
    static constexpr float textureLeafStart = 1 - 268.0f/800.0f;
    static constexpr float textureLeafEnd = 1.0f;
private:
    std::vector<glm::vec3>* leafVertices;
    std::vector<GLuint>* leafIndices;
    std::vector<glm::vec2>* leafUVs;

    float lineHeight = 0;
    int baseVerticesSize = 0;
    int heightCount = 0;
    int seed = 0;
public:
    LeafContainerAB(std::vector<glm::vec3>* leafVertices,
                        std::vector<GLuint>* leafIndices,
                        std::vector<glm::vec2>* leafUVs,
                        const int& seed);

    void buildVertices(const int& randomSeedValue, float lineSegments,
                   int lineMax, float trunkDiameter, const float& seed,
                   float lineHeight, int* baseVerticesSize);
  
    void buildContainer(float trunkDiameter, const float& seed, float lineHeight, const int& lineMax);
    
    static void buildLeafContainerElements(const int& start, const int& end,
                                           std::vector<GLuint>* leafIndices, std::vector<glm::vec3>* leafVert,
                                           std::vector<glm::vec2>* leafUVs, std::vector<glm::vec3>* leafNorms);
};


#endif //PROCEDURALWORLD_LEAFCONTAINER_H
