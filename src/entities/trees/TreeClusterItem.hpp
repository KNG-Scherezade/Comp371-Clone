//
// Created by Tof on 2017-11-20.
//

#ifndef PROCEDURALWORLD_TREECLUSTERITEM_H
#define PROCEDURALWORLD_TREECLUSTERITEM_H


#define START_TRUNK 0
#define TRUNK 2
#define LEAF 3
#define END_TRUNK -1

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "src/entities/DrawableEntity.hpp"
#include "Tree.hpp"
#include "src/TreeRandom.hpp"

#include <iostream>
#include <cstdio>
#include <ctime>

class TreeClusterItem : public Tree {
private:
    const std::string textureMap = "../textures/TreeCTexture.jpg";
    bool isAlien;
    float boostFactor = 4.2;
    int k = 600;

    int colorType;

    const float boostReduction =boostFactor / 10 ;
    const int kReduction =  k/100;
    static constexpr int branches = 1;
    static constexpr float zeroSize = 0.1;
    static constexpr int minYBranchAngle = 25;
    static constexpr int maxYBranchAngle = 45;
    static constexpr int minYTrunkAngle = 0;
    static constexpr int maxYTrunkAngle = 25;
    static constexpr int heightChunking = 20;//INVERSE
    static constexpr int trunkPoints = 4;
    static constexpr int leafPoints = 4;

    static constexpr double trunkRatio = 1.0;
    static constexpr double branchRatio = 0.850;

    int depth = 0;
    float trunkDiameter;
    int previousRotation;

    float widthCutoff = 0.05;

    float finalCutoff;
    bool kill;

    bool treeLoaded = false;

    void generateTreeCI(const int& _case, float trunkDiameter, const float& seed,
                        float angleX, float angleY, float angleZ,
                        char tag, AttatchmentGroupings* ag, float lineHeight);

    float trunk(const float& trunkDiameter, const float& seed, float lineHeight);

    void leafBranch(const float& trunkDiameter, const float& seed, float lineHeight);

    void initiateMove(AttatchmentGroupings* ag);

    void moveSegments(int rotationPoint, AttatchmentGroupings* ag);

    void connectSegments(AttatchmentGroupings* ag, const int& m);

    void bufferObject(const GLuint& shader_program);

    bool treeSetup(const GLuint& shader_program, const float& trunkDiameter, const float& seed);

public:

    float xPos;
    float zPos;

    void setLocationFromCenter(const float& circleAngle, const float& distanceFromCenter);

    void setLocationWithPoints(const float& xPos, const float& zPos);

    float getTrunkDiameter();

    TreeClusterItem(const GLuint& shader_program, Entity* entity, float trunkDiameter, float seed, bool isAlien);

    GLuint getTextureId();
    const int getColorType();
};

#endif //PROCEDURALWORLD_TREECLUSTERITEM_H
