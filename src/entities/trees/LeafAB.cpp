#include <iostream>
#include "LeafAB.hpp"

//builds 2
void LeafAB::buildLeaf(const float& r1, const float& r2, const float& leafDiameter,
                       const float& lineHeight, const int& count, float leafTextureStart){
    //place leaves parralel following angle
    //place approproate leaves parralel following angle
    for(int side = 0 ; side < 2 ; side++){
        for (int n = 0; n < leafPoints; n++) {
            //if even create a leaf with constant Z
            if (count % 2 == 0)
                leafVertices->push_back(
                        glm::vec3(
                                r2 * cos(glm::radians(itterations  * n)) + leafDiameter + r2 ,
                                lineHeight + r1 *  sin(glm::radians(itterations  * n)) ,
                                ((float)side) / 10.0f));
                //else constant X
            else
                leafVertices->push_back(
                        glm::vec3(
                                ((float)side) / 10.0f,
                                lineHeight + r1 *  sin(glm::radians(itterations  * n)) + 0,
                                r2 * cos(glm::radians(itterations  * n)) + leafDiameter / 2.0 + r2));
        }
        unsigned  long s = leafVertices->size();
        leafUVs->resize(s);
        leafUVs->at(s - 1 - 3) = {1.0, 0 + (1 - leafTextureStart) * 0.5f};
        leafUVs->at(s - 1 - 2) = {0.5, 1 - leafTextureStart};
        leafUVs->at(s - 1 - 1) = {0,  (1 - leafTextureStart) * 0.5f};
        leafUVs->at(s - 1 - 0) = {0.5, 0};
    }
    for(int side = 0 ; side < 2 ; side++) {
        for (int n = 0; n < leafPoints; n++) {
            if (count % 2 == 0)
                leafVertices->push_back(
                        glm::vec3(
                                -r2 * cos(glm::radians(itterations * n)) - leafDiameter / 2.0 - r2,
                                lineHeight + r1 * sin(glm::radians(itterations * n)),
                                ((float)side) / 10.0f));
            else
                leafVertices->push_back(
                        glm::vec3(
                                ((float)side) / 10.0f,
                                lineHeight + r1 * sin(glm::radians(itterations * n)) + 0,
                                -r2 * cos(glm::radians(itterations * n)) - leafDiameter / 2.0 - r2));

        }
        unsigned  long s = leafVertices->size();
        s = leafVertices->size();
        leafUVs->resize(s);
        leafUVs->at(s - 1 - 3) = {1.0, 0 + (1 - leafTextureStart) * 0.5f};
        leafUVs->at(s - 1 - 2) = {0.5, 1 - leafTextureStart};
        leafUVs->at(s - 1 - 1) = {0, (1 - leafTextureStart) * 0.5f};
        leafUVs->at(s - 1 - 0) = {0.5, 0};
    }
}

void LeafAB::buildLeafSingle(const float&  r1, const float&  r2, const float&  trunkDiameter, int y, float leafTextureStart) {
    for(int side = 0 ; side < 2 ; side++) {
        for (int n = 0; n < leafPoints; n++) {
            int sign = -1;
            int jagednessRandom = this->randomSeedValue * (((int) (n * 13.4) % 17) + 1);
            if (jagednessRandom % 2 == 0) sign *= -1;
            float tempTrunkDiameter = trunkDiameter +
                                      sign * (jagednessRandom) % ((int) (ceil(trunkDiameter))) * jagednessFactor_Leaf /
                                      (trunkDiameter);
            leafVertices->push_back(glm::vec3(r1 * sin(glm::radians(itterations * n)),
                                              r2 * cos(glm::radians(itterations * n)) + lineHeight + r2,
                                              ((float)side) / 10.0f));
        }
        unsigned long s = leafVertices->size();
        leafUVs->resize(s);
        leafUVs->at(s - 1 - 3) = {1.0, 0 + (1 - leafTextureStart) * 0.5f};
        leafUVs->at(s - 1 - 2) = {0.5, 1 - leafTextureStart};
        leafUVs->at(s - 1 - 1) = {0, (1 - leafTextureStart) * 0.5f};
        leafUVs->at(s - 1 - 0) = {0.5, 0};
    }
}

GLuint LeafAB::buildElements(int i, const int& start, const int& leafPoints,
                         std::vector<GLuint>* leafIndices, std::vector<glm::vec3>* leafVert,
                         std::vector<glm::vec2>* leafUVs, std::vector<glm::vec3>* leafNorms){
    glm::vec3 leafNormal = (glm::cross(
            leafVert->at(i + 2+ start) - leafVert->at(i+1 + start),
            leafVert->at(i +3+ start) - leafVert->at(i+1 + start)
    ));

    if(leafVert->size() != leafNorms->size()){
        leafUVs->resize(leafVert->size());
        leafNorms->resize(leafVert->size());
    }
    //NORTH ON ONE SIDE
    leafIndices->push_back(i + 0 + start);
    leafIndices->push_back(i + 1+ start);
    leafIndices->push_back(i + 2+ start);
    leafIndices->push_back(i + 2+ start);
    leafIndices->push_back(i + 3+ start);
    leafIndices->push_back(i +  0 + start);
    //SOUTH ON THE OTHER
    leafIndices->push_back(i + 4 + start);
    leafIndices->push_back(i + 7 + start);
    leafIndices->push_back(i + 6 + start);
    leafIndices->push_back(i + 6 + start);
    leafIndices->push_back(i + 5 + start);
    leafIndices->push_back(i + 4 + start);
    leafNorms->at(i + 0 + start) = (leafNormal);
    leafNorms->at(i + 1 + start) = (leafNormal);
    leafNorms->at(i + 2 + start) = (leafNormal);
    leafNorms->at(i + 3 + start) = (leafNormal);

    leafNorms->at(i + 4 + start) = -(leafNormal);
    leafNorms->at(i + 5 + start) = -(leafNormal);
    leafNorms->at(i + 6 + start) = -(leafNormal);
    leafNorms->at(i + 7 + start) = -(leafNormal);

    return i = i + leafPoints * 2;
}



LeafAB::LeafAB(const int& leafPoints, const int&  randomSeedValue,
                      const float& itterations, const float& jagednessFactor_Leaf,
                        std::vector<glm::vec3>* leafVertices, std::vector<glm::vec2>* leafUVs) {
    this->leafPoints = leafPoints;
    this->randomSeedValue = randomSeedValue;
    this->itterations = itterations;
    this->jagednessFactor_Leaf = jagednessFactor_Leaf;
    this->leafUVs = leafUVs;
    this->leafVertices = leafVertices;
};