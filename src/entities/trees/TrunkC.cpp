
#ifndef PROCEDURALWORLD_TRUNKC_C
#define PROCEDURALWORLD_TRUNKC_C


#include <iostream>
#include "TrunkC.hpp"

bool TrunkC::constructionFlowCounter = true;

TrunkC::TrunkC(std::vector<glm::vec3>* combinedVertices, std::vector<glm::vec2>* combinedUV, std::vector<glm::vec3>* combinedNormals,
               std::vector<GLuint>* combinedIndices,
               float lineMax, float textureHeight){
    this->combinedVertices = combinedVertices;
    this->combinedNormals = combinedNormals;
    this->combinedUV = combinedUV;
    this->textureHeight = textureHeight;
    this->combinedIndices = combinedIndices;
    this->lineMax = lineMax;
}

float TrunkC::buildAllComponents(const float& trunkDiameter, const float& seed, float lineHeight){
    int randomSeedValue = TreeRandom::treeRandom(trunkDiameter, seed, lineHeight);
    bool loopInitialTrunk = true;
    const float lineSegments = lineMax / heightChunking;
    const int baseVerticesSize = combinedVertices->size();
    int count = 0;
    const float itterations = 360.0f / trunkPoints;
    //MOVE PROCEEDURE TO TRUNK CLASS
    do {
        //build points
        for (int y = 0; y < 3; y++) {
            float uCounter =  0;
            for (int n = 0; n < trunkPoints; n++) {
                int sign = -1;
                const int jagednessRandom = randomSeedValue  * (((int)(n * 13.4) % 17) + 1 + y);
                if (jagednessRandom % 2 == 0) sign *= -1;
                float tempTrunkDiameter = trunkDiameter + sign * (jagednessRandom) % ((int)(ceil(trunkDiameter))) * jagednessFactor / (trunkDiameter);
                glm::vec3 circleEdge(
                        tempTrunkDiameter * sin(glm::radians(itterations  * n)) ,
                        lineHeight + 0,
                        tempTrunkDiameter *  cos(glm::radians(itterations  * n))
                );
                combinedVertices->push_back(circleEdge);
                unsigned  long s = combinedVertices->size();
                combinedUV->resize(s);
                combinedUV->at(s - 1) = {uCounter, 1 - textureHeight * ((y + (!TrunkC::constructionFlowCounter )) % 2) };
                uCounter += 0.33;
            }
            lineHeight += lineSegments;
            randomSeedValue = TreeRandom::treeRandom(trunkDiameter, seed, lineHeight);
        }
        //build indices
        for (int y = 0; y < 2; y++) {
            std::vector<glm::vec3> surfaceNormals;
            const int offset = (y + 1) * trunkPoints + baseVerticesSize;
            const int base = y * trunkPoints + baseVerticesSize;
            if (count > 0 && y == 0) {
                for (int n = 0; n < trunkPoints; n++) {
                    int countTemp = count - 1;
                    combinedIndices->push_back(base + n + countTemp * trunkPoints);
                    combinedIndices->push_back(base + (n + 1) % trunkPoints + countTemp * trunkPoints);
                    combinedIndices->push_back(offset + (n + 1) % trunkPoints + countTemp * trunkPoints);
                    combinedIndices->push_back(offset + (n + 1) % trunkPoints + countTemp * trunkPoints);
                    combinedIndices->push_back(offset + n + countTemp * trunkPoints);
                    combinedIndices->push_back(base + n + countTemp * trunkPoints);
                    // calculate normals
                    //------------------

                    //step 1: find the surface normal
                    glm::vec3 AB = combinedVertices->at(base + (n + 1) % trunkPoints + countTemp * trunkPoints) -
                                   combinedVertices->at(base + n + countTemp * trunkPoints);
                    glm::vec3 AC = combinedVertices->at(offset + n + countTemp * trunkPoints) -
                                   combinedVertices->at(base + n + countTemp * trunkPoints);

                    glm::vec3 surfaceNormal = -glm::cross(AB, AC);
                    surfaceNormals.push_back(surfaceNormal);
                }
                //step 2: find the average of the surface normals of the surfaces this vertex is part of
                int len =  surfaceNormals.size();
                for(GLuint i = 0; i < len ; i++){
                    int iPos = base + i + (count - 1) * trunkPoints + 1;
                    if(iPos % trunkPoints == 0) iPos =  base + 0 + (count - 1) * trunkPoints;
                    combinedNormals->at(iPos) = -glm::normalize(
                            surfaceNormals.at(i) + surfaceNormals.at((i+1) % len)
                    );
                }
            }
            for (int n = 0; n < trunkPoints; n++) {
                //WORKS
                combinedIndices->push_back(base + n + count * trunkPoints);
                combinedIndices->push_back(base + (n + 1) % trunkPoints + count * trunkPoints);
                combinedIndices->push_back(offset +(n + 1)% trunkPoints + count * trunkPoints);
                combinedIndices->push_back(offset + (n + 1)% trunkPoints + count * trunkPoints);
                combinedIndices->push_back(offset + n + count * trunkPoints);
                combinedIndices->push_back(base + n + count * trunkPoints);

                unsigned  long s = combinedVertices->size();
                combinedNormals->resize(s);
                combinedNormals->at(base + n + count * trunkPoints) = glm::cross(
                        combinedVertices->at(base + (n + 1) %  trunkPoints + count * trunkPoints) - combinedVertices->at(base + n %   trunkPoints+ count * trunkPoints),
                        combinedVertices->at(offset + n + count * trunkPoints) - combinedVertices->at(base + n %  trunkPoints + count * trunkPoints)
                );
                // calculate normals
                //------------------

                //step 1: find the surface normal
                glm::vec3 AB = combinedVertices->at(base + (n + 1) % trunkPoints  + count * trunkPoints)    -  combinedVertices->at(base + n + count* trunkPoints);
                glm::vec3 AC = combinedVertices->at(offset + n + count* trunkPoints)    -   combinedVertices->at(base + n + count * trunkPoints);

                glm::vec3 surfaceNormal = -glm::cross(AB, AC);
                surfaceNormals.push_back(surfaceNormal);
            }

            //step 2: find the average of the surface normals of the surfaces this vertex is part of
            int len =  surfaceNormals.size();
            for(GLuint i = 0; i < len ; i++){
                int iPos = base + i + count * trunkPoints + 1;
                if(iPos % trunkPoints == 0) iPos = base + 0 + count * trunkPoints;
                combinedNormals->at(iPos  ) = -glm::normalize(
                        surfaceNormals.at(i) + surfaceNormals.at((i+1) % len)
                );
            }
        }
        count += 3;
        TrunkC::constructionFlowCounter = !TrunkC::constructionFlowCounter;
        if ((randomSeedValue * (count + 1)) % branchMod == 0) {
            loopInitialTrunk = false;
        }
    } while (loopInitialTrunk && lineHeight < lineMax);
    //TrunkC::constructionFlowCounter = !TrunkC::constructionFlowCounter;
    if (lineHeight >= lineMax)
        return -1;
    else if(!loopInitialTrunk)
        return lineHeight;
    else return -1;
}

#endif //PROCEDURALWORLD_TRUNKC_H
