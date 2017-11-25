
#ifndef PROCEDURALWORLD_LEAF_H
#define PROCEDURALWORLD_LEAF_H

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

class LeafAB {
    int leafPoints;
    int randomSeedValue;
    int count;
    float lineHeight;
    float angle;
    float jagednessFactor_Leaf;
    std::vector<glm::vec3> *leafVertices;
    std::vector<GLuint> *leafIndices;
    std::vector<glm::vec2> *leafUVs;
public:
    LeafAB(const int& leafPoints, const int&  randomSeedValue, const int&  count, const float& lineHeight,
          const float& itterations, const float& jagednessFactor_Leaf, std::vector<glm::vec3>* leafVertices,
                   std::vector<GLuint>* leafIndices, std::vector<glm::vec2>* leafUVs);

    //builds 2
    void buildLeaf(const float& r1, const float& r2, const float& leafDiameter, const int& count);

    static GLuint buildElements(int i, const int& start, const int& leafPoints,
                                const float& leafABTextureStartPos , glm::vec3 leafNormal,
                             std::vector<GLuint>* leafIndices, std::vector<glm::vec3>* leafVert,
                             std::vector<glm::vec2>* leafUVs, std::vector<glm::vec3>* leafNorms);

    void buildLeafSingle(const float& r1, const float& r2, const float& trunkDiameter);
};
#endif //leaf