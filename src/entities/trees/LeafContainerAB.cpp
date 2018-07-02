/*
 *     ^
 *  <-[]->
 *    [x]
 * <--[-]-->
 *    [x]
 * <--[-]-->
 *
 * Goes up alternating leaf angle ending with a top leaf
 * Double sided index on leaves
 */

#include <cmath>

#include "LeafContainerAB.hpp"

constexpr int LeafContainerAB::leafPoints;
constexpr int LeafContainerAB::leafBranchPoints;

constexpr float LeafContainerAB::itterationsLeaf;
constexpr float LeafContainerAB::itterationsLeafBranch;

constexpr float LeafContainerAB::jagednessFactor_Leaf;
constexpr float LeafContainerAB::textureLeafStart;
constexpr float LeafContainerAB::textureLeafEnd;

LeafContainerAB::LeafContainerAB(std::vector<glm::vec3>* leafVertices,
                                 std::vector<GLuint>* leafIndices,
                                 std::vector<glm::vec2>* leafUVs,
                                 const int& seed){
    this->leafVertices = leafVertices;
    this->leafIndices = leafIndices;
    this->leafUVs = leafUVs;
    baseVerticesSize = leafVertices->size();
    this->seed = seed;
}

//method to encapsulate the construction.
void LeafContainerAB::buildContainer(float trunkDiameter, const float& seed, float lineHeight, const int& lineMax){
    int randomSeedValue = TreeRandom::treeRandom(trunkDiameter, seed, lineHeight);

    const float lineSegments = trunkDiameter * 3.0;
    int baseVerticesSize = leafVertices->size();

    //leaves are in altrenating 90 degrees and stord on a branch
    buildVertices(randomSeedValue, lineSegments, lineMax, trunkDiameter, seed, lineHeight, &baseVerticesSize);
}
//uses leafbranch and leaf classes to construct vertices
void LeafContainerAB::buildVertices(const int& randomSeedValue, float lineSegments,
                                    int lineMax, float trunkDiameter, const float& seed,
                                    float lineHeight, int* baseVerticesSize){

    const float branchItterations = 360.0f / leafBranchPoints;
    const float leafItterations = 360.0f/ leafPoints;

    //boost line length
    lineSegments *= 1.5;
    //elipse radius values
    float r2 = 0.000176135f * sqrt(std::abs(-88063572 + 50843527 * trunkDiameter * lineSegments)) ;
    float r1 = sqrt(3) * sqrt(std::abs(2 - r2 * r2));
    r1 = floor(r1) == 0 ? 1:r1;

    int count = 0;


    LeafAB LA(leafPoints, randomSeedValue, branchItterations, jagednessFactor_Leaf,
              leafVertices, leafUVs);

    while (lineHeight < lineMax - 2){
        //branch
        LeafBranchAB LBA(randomSeedValue, branchItterations, leafBranchPoints,
                         lineHeight, jagednessFactor_Leaf, leafVertices, nullptr,leafUVs);
        LBA.buildLeafBranch(trunkDiameter, count, textureLeafStart);
        trunkDiameter *= 0.9;
        //builds 2 leaves at alternating 90 degrees. Uses count to do this.

        //north and south
        LA.buildLeaf(r1, r2, trunkDiameter, lineHeight, count, textureLeafStart); //build two double sided leaves
        lineHeight += lineSegments;
        count++;
        //REPEAT NX
    }
    TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;

    for (float y = 1.0; y >= 0.25; y -= 0.25) {
        //the branch is getting smaller
        LeafBranchAB LBA(randomSeedValue, branchItterations, leafBranchPoints,
                         lineHeight, jagednessFactor_Leaf, leafVertices, nullptr, leafUVs);
        LBA.buildLeafBranch(trunkDiameter, count, textureLeafStart);

        //still making leaves
        LA.buildLeaf(r1, r2, trunkDiameter, lineHeight, count, textureLeafStart);  //build two double sided leaves
        lineHeight += lineSegments;
        trunkDiameter *= y ;
        count++;
        //REPEAT 2X
    }
    LeafBranchAB LBA(randomSeedValue, branchItterations, leafBranchPoints,
                     lineHeight, jagednessFactor_Leaf, leafVertices, nullptr, leafUVs);

    LBA.buildLeafBranch(trunkDiameter*0.1, count, textureLeafStart);

    //final top two leaves
    LA.buildLeaf(r1, r2, trunkDiameter, lineHeight, count, textureLeafStart);  //build two double sided leaves
    //Cap with a leaf
    LA.buildLeafSingle(r1, r2, r1, count, textureLeafStart);
}


void LeafContainerAB::buildLeafContainerElements(const int& start, const int& end,
                                                 std::vector<GLuint>* leafIndices, std::vector<glm::vec3>* leafVert,
                                                 std::vector<glm::vec2>* leafUVs, std::vector<glm::vec3>* leafNorms){

    const int endVert = (end - start - leafPoints*2)/ (leafBranchPoints + leafPoints*4);
    const int vertToNext = ((1) *(leafBranchPoints + leafPoints*4));
    const  int factor = (leafBranchPoints + leafPoints*4);

    GLuint i = 0;
    //build a running set of vertices to add indices to.
    for(int k = 0 ; k < endVert; k++){
        std::vector<glm::vec3> surfaceNormals;
        int j = i;
        for(; i < leafBranchPoints + j ; i++) {
            const int currentVertSet = ((k) *factor);
            GLuint i1 = (i + 1) % (factor) % leafBranchPoints +  currentVertSet;
            leafIndices->push_back(i + start);
            leafIndices->push_back(i1+ start);
            leafIndices->push_back(i1 + vertToNext + start);
            leafIndices->push_back(i1  +  vertToNext + start);
            leafIndices->push_back(i  +  vertToNext + start);
            leafIndices->push_back(i+ start);
            // calculate normals
            //------------------
            //step 1: find the surface normal
            glm::vec3 AB = leafVert->at(i1+ start)    -  leafVert->at(i+ start);
            glm::vec3 AC = leafVert->at(i + vertToNext +  start)    -   leafVert->at(i+ start);

            glm::vec3 surfaceNormal = -glm::cross(AB, AC);
            surfaceNormals.push_back(surfaceNormal);
        }
        //step 2: find the average of the surface normals of the surfaces this vertex is part of
        int len =  surfaceNormals.size();
        for(GLuint j = 0; j < len ; j++){
            int jPos = i + start - leafBranchPoints + j + 1;
            if(jPos % leafPoints == 0) jPos =  i + start - leafBranchPoints + 0;
            leafNorms->at(jPos ) = -glm::normalize(
                    surfaceNormals.at(j) + surfaceNormals.at((j+1) % len)
            );
        }

        i = LeafAB::buildElements(i, start, leafPoints ,leafIndices, leafVert,
                                 leafUVs, leafNorms);
        i = LeafAB::buildElements(i, start, leafPoints , leafIndices, leafVert,
                                 leafUVs, leafNorms);
    }
    i+= leafPoints;

    //incides and normal of top leaf
    i = LeafAB::buildElements(i, start,  leafPoints,  leafIndices, leafVert,
                             leafUVs, leafNorms);
    i = LeafAB::buildElements(i, start,  leafPoints,  leafIndices, leafVert,
                             leafUVs, leafNorms);
    //
    i = LeafAB::buildElements(i, start,  leafPoints,  leafIndices, leafVert,
                             leafUVs, leafNorms);
}

