
#ifndef PROCEDURALWORLD_TRUNKC_H
#define PROCEDURALWORLD_TRUNKC_H

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

class TrunkC {
    std::vector<glm::vec3>* combinedVertices;
    std::vector<glm::vec2>* combinedUV;
    std::vector<glm::vec3>* combinedNormals;
    std::vector<GLuint>* combinedIndices;

    float textureHeight;

    static constexpr int heightChunking = 20;
    static constexpr int trunkPoints = 4;
    static constexpr int branchMod  = 10;

    static constexpr float jagednessFactor = 0.20;
    float lineMax;
public:
    static bool constructionFlowCounter;

    TrunkC(std::vector<glm::vec3>* combinedVertices, std::vector<glm::vec2>* combinedUV, std::vector<glm::vec3>* combinedNormals, std::vector<GLuint>* combinedIndices, float textureHeight,
           float lineMax);
    float buildAllComponents(const float& trunkDiameter, const float& seed, float lineHeight);
};


#endif //PROCEDURALWORLD_TRUNKC_H
