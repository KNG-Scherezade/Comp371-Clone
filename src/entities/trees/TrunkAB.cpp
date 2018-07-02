#include <cmath>
#include "TrunkAB.hpp"

bool TrunkAB::constructionFlowCounter = true;

TrunkAB::TrunkAB(std::vector<glm::vec3>* trunkVertices, std::vector<glm::vec2>* trunkUVs,  const int& seed){
    this->trunkVertices = trunkVertices;
    this->trunkUVs = trunkUVs;
    baseVerticesSize = trunkVertices->size();
    this->seed = seed;
}

bool TrunkAB::buildTrunk(const float& trunkDiameter, const float& lineSegments){
    const int randomSeedValue = buildVertices(trunkDiameter, lineSegments);
    heightCount += 3;
    //make branch check
    if ((randomSeedValue * (heightCount + 1)) % branchMod == 0) {
        return false;
    }
    return true;
}

int TrunkAB::buildVertices(const float& trunkDiameter, const float& lineSegments){
    //build points
    const int randomSeedValue = TreeRandom::treeRandom(trunkDiameter, seed, lineHeight);
    for (int y = 0; y < 3; y++) {
        for (int n = 0; n < trunkPoints; n++) {
            int sign = -1;
            const int jagednessRandom = randomSeedValue  * (((int)(n * 13.4) % 17) + 1 + y);
            if (jagednessRandom % 2 == 0) sign *= -1;
            float tempTrunkDiameter = trunkDiameter + sign * (jagednessRandom) % ((int)(ceil(trunkDiameter)))
                                                      * jagednessFactor / (trunkDiameter);
            glm::vec3 circleEdge(
                    tempTrunkDiameter * sin(glm::radians(itterations  * n)),
                    lineHeight + 0, tempTrunkDiameter *  cos(glm::radians(itterations  * n)) );
            trunkVertices->push_back(circleEdge);
            unsigned  long s = trunkVertices->size();
            trunkUVs->resize(s);
            trunkUVs->at(s - 1) = {(n) % 2, 1 - textureTrunkHeight * ((y + (!constructionFlowCounter)) % 2)};
        }
        lineHeight += lineSegments;
    }
    TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;
    return randomSeedValue;
}
//give both start point and end as the connection end point
void TrunkAB::buildTrunkElements(const int& start, const int& end,
                                std::vector<GLuint>* trunkIndices, std::vector<glm::vec3>* trunkVert,
                                std::vector<glm::vec3>* trunkNorms){
    if(trunkVert->size() != trunkNorms->size()){
        trunkNorms->resize(trunkVert->size());
    }

    std::vector<glm::vec3> surfaceNormals;
    for (GLuint i = start; i < end - trunkPoints + 1; i++) {
        GLuint i1 = (i + 1) % trunkPoints + (i / trunkPoints * trunkPoints);

        trunkIndices->push_back(i);
        trunkIndices->push_back(i1);
        trunkIndices->push_back(i1 + trunkPoints);
        trunkIndices->push_back(i1 + trunkPoints);
        trunkIndices->push_back(i + trunkPoints);
        trunkIndices->push_back(i);

        // calculate normals
        //------------------

        //step 1: find the surface normal
        glm::vec3 AB = trunkVert->at(i1)    -  trunkVert->at(i);
        glm::vec3 AC = trunkVert->at(i + trunkPoints)    -   trunkVert->at(i);

        glm::vec3 surfaceNormal = -glm::cross(AB, AC);
        surfaceNormals.push_back(surfaceNormal);
    }

    //step 2: find the average of the surface normals of the surfaces this vertex is part of
    int len =  surfaceNormals.size();
        for(GLuint i = 0; i < len ; i++){
            int iPos = i + start + 1;
            if(iPos % trunkPoints == 0) iPos = i + start + 1 - trunkPoints;
            trunkNorms->at(iPos  ) = -glm::normalize(
                    surfaceNormals.at(i) + surfaceNormals.at((i+1) % len)
            );
    }
}

void TrunkAB::buildConnectorElements(const int& segmentConnectStart,const int& start, const int& set, const char& lr,
                                   std::vector<GLuint>* trunkIndices, std::vector<glm::vec3>* trunkVert,
                                   std::vector<glm::vec3>* trunkNorms){
    if(trunkVert->size() != trunkNorms->size()){
        trunkNorms->resize(trunkVert->size());
    }

    std::vector<glm::vec3> surfaceNormals;

    //Norms to top segment starting from lower circle
    for(GLuint i = 0 ; i < trunkPoints; i++){
        GLuint i1 = (i + 1) % trunkPoints;

        trunkIndices->push_back(segmentConnectStart + (i % trunkPoints));
        trunkIndices->push_back(segmentConnectStart + (i+1) % trunkPoints);
        //start implies top, or start of top segment
        trunkIndices->push_back(start + (i + 1 + set) % trunkPoints);
        trunkIndices->push_back(start + (i + 1 + set) % trunkPoints);
        trunkIndices->push_back(start + (i + set) % trunkPoints);
        trunkIndices->push_back(segmentConnectStart + i % trunkPoints);

        // calculate normals
        //------------------

        //step 1: find the surface normal
        glm::vec3 AB = trunkVert->at(segmentConnectStart + (i + 1) % trunkPoints)    -   trunkVert->at(segmentConnectStart + i % trunkPoints);
        glm::vec3 AC = trunkVert->at(start +  (i + set) % trunkPoints)    -   trunkVert->at(segmentConnectStart + i % trunkPoints);

        glm::vec3 surfaceNormal = -glm::cross(AB, AC);
        surfaceNormals.push_back(surfaceNormal);

    }

    //step 2: find the average of the surface normals of the surfaces this vertex is part of
    int len =  surfaceNormals.size();
    for(GLuint i = 0; i < len ; i++){
        int iPos = i + segmentConnectStart + 1;
        if(iPos % trunkPoints == 0) iPos = i + segmentConnectStart + 1 - trunkPoints;
        trunkNorms->at(iPos  ) = -glm::normalize(
                surfaceNormals.at(i) + surfaceNormals.at((i+1) % len)
        );
    }
}

float TrunkAB::getLineHeight(){return lineHeight;}