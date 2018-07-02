#ifndef COMP371_TRUNK_A_HPP
#define COMP371_TRUNK_A_HPP
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

#include <iostream>
#include <cstdio>
#include <ctime>

class TrunkAB{
public:
    static bool constructionFlowCounter;

    static constexpr int branchMod = 1;
    static constexpr int trunkPoints = 4;
    static constexpr float itterations = 360.0f / trunkPoints;
private:

    static constexpr float jagednessFactor = 0.4;
    static constexpr float textureTrunkHeight = 530.0/800.0;
    static constexpr float textureConnectorStart = 0.75;
    static constexpr float textureConnectorEnd = 0.90;

    std::vector<glm::vec3>* trunkVertices;
    std::vector<glm::vec2>* trunkUVs;
    float lineHeight = 0;
    int baseVerticesSize = 0;
    int heightCount = 0;
    int seed = 0;

    int buildVertices(const float& trunkDiameter, const float& lineSegments);

public:

    float getLineHeight();

    TrunkAB(std::vector<glm::vec3>* trunkVertices ,std::vector<glm::vec2>* trunkUVs, const int& seed);

    static void buildConnectorElements(const int& segmentConnectStart,const int& start, const int& set, const char& lr,
                                       std::vector<GLuint>* trunkIndices, std::vector<glm::vec3>* trunkVert,
                                        std::vector<glm::vec3>* trunkNorms);

    //give both start point and end as the connection end point
    static void buildTrunkElements(const int& start, const int& end,
                                   std::vector<GLuint>* trunkIndices, std::vector<glm::vec3>* trunkVert,
                                   std::vector<glm::vec3>* trunkNorms);
    
    bool buildTrunk(const float& trunkDiameter, const float& lineSegments);
};
#endif //COMP371_TRUNK_A_HPP