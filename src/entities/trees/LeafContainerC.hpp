
#ifndef PROCEDURALWORLD_l_H
#define PROCEDURALWORLD_l_H

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

class LeafContainerC {
    std::vector<glm::vec3>* combinedVertices;
    std::vector<glm::vec3>* combinedNormals;
    std::vector<glm::vec2>* combinedUV;
    std::vector<GLuint>* combinedIndices;

    int lineMax;
    int heightChunking;
    float leafTextureHeight;

    static constexpr int leafPoints = 4;
    static constexpr int stretchFactor = 100;
    static constexpr float jagednessFactor_Leaf = 0.0020;
    int branchMod;
public:
    LeafContainerC(std::vector<glm::vec3>* combinedVertices, std::vector<glm::vec2>* combinedUV, std::vector<glm::vec3>* combinedNormals ,std::vector<GLuint>* combinedIndices, float leafTextureHeight,
                   float lineMax);

    void buildAllComponenets(const float& leafDiameter, const float& widthCutoff, const float& seed, float lineHeight);
    void setLeafUV();
};


#endif //PROCEDURALWORLD_l_H
