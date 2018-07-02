#include "LeafBranchAB.hpp"
#include "TrunkAB.hpp"

void LeafBranchAB::buildLeafBranch(const float& trunkDiameter, int y , float textureLeafStart ){
    for (int n = 0; n < branchPoints ; n++) {
        int sign = -1;
        int jagednessRandom = randomSeedValue * (((int)(n * 13.4) % 17) + 1);
        if (jagednessRandom % 2 == 0) sign *= -1;
        float tempTrunkDiameter = trunkDiameter + sign * (jagednessRandom) % ((int)(ceil(trunkDiameter))) * jagednessFactor_Leaf / (trunkDiameter);
        leafVertices->push_back(glm::vec3(
                tempTrunkDiameter * sin(glm::radians(itterations  * n)) ,
                lineHeight,
                tempTrunkDiameter *  cos(glm::radians(itterations  * n))));
        unsigned  long s = leafVertices->size();
        leafUVs->resize(s);
        leafUVs->at(s - 1) = {(n) % 2, 1 - textureLeafStart * ((y + (!TrunkAB::constructionFlowCounter )) % 2)};
    }
}

void LeafBranchAB::buildBranchElements(int baseVerticesSize, int i, int len){
    const int base = i * branchPoints* 3 + baseVerticesSize;
    const int set = (i + 1)*branchPoints* 3 + baseVerticesSize;
    int nSolution = 0;
    for (int n = 0; n < branchPoints; n++) {
        if (n == branchPoints - 1) {
            leafIndices->push_back(base + n);
            leafIndices->push_back(base + 0);
            leafIndices->push_back(set + 0);
            leafIndices->push_back(set + 0);
            leafIndices->push_back(set + n);
            leafIndices->push_back(base + n);
            nSolution++;
        } else {
            leafIndices->push_back(base + n);
            leafIndices->push_back(base + n + 1);
            leafIndices->push_back(set + n + 1);
            leafIndices->push_back(set + n + 1);
            leafIndices->push_back(set + n);
            leafIndices->push_back(base + n);
        }
    }
}

LeafBranchAB::LeafBranchAB(const int& randomSeedValue, const float& itterations, const int& branchPoints,
            const float& lineHeight, const float& jagednessFactor_Leaf,
            std::vector<glm::vec3>* leafVertices,
            std::vector<GLuint>* leafIndices, std::vector<glm::vec2>* leafUVs){
    this->randomSeedValue = randomSeedValue;
    this->itterations = itterations;
    this->branchPoints = branchPoints;
    this->lineHeight = lineHeight;
    this->jagednessFactor_Leaf = jagednessFactor_Leaf;
    this->leafVertices = leafVertices;
    this->leafIndices = leafIndices;
    this->leafUVs = leafUVs;
}

float LeafBranchAB::getLineHeight(){
    return lineHeight;
}