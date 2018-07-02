//
// Created by Tof on 2017-11-20.
//

#include "TreeClusterItem.hpp"
#include "LeafContainerC.hpp"
#include "TrunkC.hpp"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <src/loadTexture.hpp>

constexpr int TreeClusterItem::branches;
constexpr float TreeClusterItem::zeroSize;
constexpr int TreeClusterItem::minYBranchAngle;
constexpr int TreeClusterItem::maxYBranchAngle;
constexpr int TreeClusterItem::minYTrunkAngle;
constexpr int TreeClusterItem::maxYTrunkAngle;
constexpr int TreeClusterItem::heightChunking;
constexpr int TreeClusterItem::trunkPoints;
constexpr int TreeClusterItem::leafPoints;

constexpr double TreeClusterItem::trunkRatio;
constexpr double TreeClusterItem::branchRatio;

TreeClusterItem::TreeClusterItem (const GLuint& shader_program, Entity* entity, float trunkDiameter, float seed, bool isAlien):
    Tree(heightChunking, boostFactor,seed, shader_program, entity, 'G'){

    if (trunkDiameter <= 0.0) trunkDiameter = this->zeroSize;
    this->trunkDiameter = trunkDiameter;

    this->isAlien = isAlien;

    if(isAlien) TreeClusterItem::colorType = COLOR_TREE;
    else TreeClusterItem::colorType = COLOR_LIGHTING;

    seed = TreeRandom::treeRandom(trunkDiameter, seed, 991);

    treeLoaded = treeSetup(shader_program, trunkDiameter, seed);

    float globalRotation = TreeRandom::treeRandom(trunkDiameter,seed,widthCut*100);
    rotate(globalRotation, glm::vec3(0.0f,1.0f,0.0f));

};


bool TreeClusterItem::treeSetup(const GLuint& shader_program, const float& trunkDiameter, const float& seed){
    draw_mode = GL_TRIANGLES;
    //widthCutoff
    finalCutoff = widthCutoff;
    combinedStartIndices.push_back({-1,0,0,0});
    generateTreeCI(0, trunkDiameter, seed, 0, 0, 0, 'C', nullptr, 0);
    bufferObject(shader_program);
    rotate(TreeRandom::treeRandom(trunkDiameter, seed,100), glm::vec3(0.0f,1.0f,0.0f));
    return true;
}

void TreeClusterItem::generateTreeCI(const int& _case, float trunkDiameter, const float& seed,
                                     float angleX, float angleY, float angleZ,
                                     char tag, AttatchmentGroupings* ag, float lineHeight){
    float currentLineLength = lineHeight;
    float offShootDiameterBranch = 0;
    float offShootDiameterTrunk = 0;
    glm::mat4 rotation;
    glm::vec3 translation;
    AttatchmentGroupings* agNew;
    switch (_case)
    {
        case START_TRUNK:
            //1A. Standard trunk with two branches
            //1A1. Draw circle with seed based ragedness and store in trunkV and trunkC
            //1A2. Draw circle up one with seed based ragedness and store in trunkV and trunkC
            //1A3. Draw circle up one with seed based ragedness and store in trunkV and trunkC and create trunkI
            //1A4. Make branch check
            currentLineLength = trunk(trunkDiameter, seed, currentLineLength);
            combinedStartIndices.push_back({ (int)combinedVertices.size() - 1, (int)angleX, (int)angleY, (int)angleZ});

            agNew = new AttatchmentGroupings(combinedStartIndices.at((int)combinedStartIndices.size() - 2).at(0),
                                             (int)combinedVertices.size() - 1,
                                             (int)angleX, (int)angleY, (int)angleZ, 'B', tag);

            //1A5. Start N new recursive functions from seed based angle at a certain base position
            offShootDiameterBranch = shootCalculation(trunkDiameter, branchRatio, branches);
            offShootDiameterTrunk = shootCalculation(trunkDiameter, trunkRatio, branches);

            //1A6. On new branch create junction and new shoot
            angleY = TreeRandom::treeOddEvenRandom(trunkDiameter, seed* ( 1) * 3, (lineHeight+1)* ( 1));
            for (int n = 0; n < branches; n++) {
                angleZ = TreeRandom::branchAngleFromRandom(trunkDiameter, seed * (n+1), currentLineLength* (n + 1),
                                                           maxYBranchAngle, minYBranchAngle);
                angleX = TreeRandom::branchAngleFromRandom(trunkDiameter, seed* (n + 1) * 7, currentLineLength * (n + 1),
                                                           maxYBranchAngle, minYBranchAngle); //* (((int)seed) % 2 == 0 ? -1 : 1);
                angleY = angleY;
                depth = 0;
                generateTreeCI(TRUNK, offShootDiameterBranch / (branches), seed, -std::abs(angleX), angleY, std::abs(angleZ), 'R', agNew, 0);
                TrunkC::constructionFlowCounter=!TrunkC::constructionFlowCounter;
            }
            //1A7. On new trunk join to junction and continue
            angleZ = TreeRandom::trunkAngleFromRandom(trunkDiameter, seed* 71, currentLineLength, maxYTrunkAngle, minYTrunkAngle); ;
            angleX = TreeRandom::trunkAngleFromRandom(trunkDiameter, seed* 71, currentLineLength, maxYTrunkAngle, minYTrunkAngle);; //* (((int)seed) % 2 == 0 ? 1 : -1);
            angleY = angleY;
            depth = 0;
            previousRotation = 0;
            //CONSIDERATION: MULTITHREAD THE TRUNK AND MOVEMENT
            generateTreeCI(TRUNK, offShootDiameterTrunk, seed, std::abs(angleX), angleY, -std::abs(angleZ), 'L', agNew, currentLineLength);
            TrunkC::constructionFlowCounter=!TrunkC::constructionFlowCounter;
            initiateMove(agNew);
            agNew->selfErase();
            delete agNew;

            break;
        case TRUNK:
            k -= kReduction;
            depth++;
            boostFactor -= boostReduction;
            kill = false;
            if (trunkDiameter < widthCutoff) {
                generateTreeCI(LEAF, trunkDiameter, seed, angleX, angleY, angleZ, tag, ag, 0);
                if (trunkDiameter < finalCutoff) {
                    k += kReduction;
                    depth--;
                    boostFactor += boostReduction;
                    kill = true;
                    return;
                }
            }
            else{
                currentLineLength = trunk(trunkDiameter, seed, lineHeight);
            }

            //rotate based on the previous and current angles
            angleX += ag->angleX;
            angleY = angleY;
            angleZ += ag->angleZ;

            //add the sum of angles onto the current branch
            combinedStartIndices.push_back({ (int)combinedVertices.size() - 1, (int)angleX, (int)angleY, (int)angleZ });

            //store current branch poosition and rotation sum at depth
            agNew = new AttatchmentGroupings(combinedStartIndices.at((int)combinedStartIndices.size() - 2).at(0),
                                             (int)combinedVertices.size() - 1, (int)angleX, (int)angleY, (int)angleZ, 'B', tag);
            if (tag == 'R') ag->ag[1] = agNew;
            else			ag->ag[0] = agNew;

            //1A5. Start N new recursive functions from seed based angle at a certain base position
            angleY = TreeRandom::treeRandom(trunkDiameter, seed * 5, lineHeight);
            if (lineHeight == -1 || currentLineLength == -1) {
                //if(//Test)//("====SPLIT====\n");
                offShootDiameterTrunk = shootCalculation(trunkDiameter, trunkRatio, branches);
                lineHeight = 0; currentLineLength = 0;
            }
            else{
                offShootDiameterBranch = shootCalculation(trunkDiameter, branchRatio, branches);
                offShootDiameterTrunk = shootCalculation(trunkDiameter, trunkRatio, branches);
                for (int n = 0; n < branches; n++) {
                    //1A6. On new branch create circle then indices flowing back once to center cirlce.
                    angleZ = TreeRandom::branchAngleFromRandom(trunkDiameter, seed, currentLineLength,
                                                               maxYBranchAngle, minYBranchAngle);
                    angleX = TreeRandom::branchAngleFromRandom(trunkDiameter, seed, currentLineLength,
                                                               maxYBranchAngle, minYBranchAngle) * (((int)seed) % 2 == 0 ? -1 : 1);;
                    angleY = angleY;
                    generateTreeCI(TRUNK, offShootDiameterBranch, seed, -std::abs(angleX), angleY, std::abs(angleZ), 'R', agNew, 0);
                    TrunkC::constructionFlowCounter=!TrunkC::constructionFlowCounter;
                }
            }

            //1A7. On new trunk create circle then indices flowing back once to top circle.
            angleZ = TreeRandom::trunkAngleFromRandom(trunkDiameter, seed, currentLineLength, maxYTrunkAngle, minYTrunkAngle);;
            angleX = TreeRandom::trunkAngleFromRandom(trunkDiameter, seed * 7, currentLineLength, maxYTrunkAngle, minYTrunkAngle) * (((int)seed) % 2 == 0 ? -1 : 1);;
            angleY = angleY;
            generateTreeCI(TRUNK, offShootDiameterTrunk, seed, std::abs(angleX), angleY, -std::abs(angleZ), 'L', agNew, currentLineLength);
            TrunkC::constructionFlowCounter=!TrunkC::constructionFlowCounter;
            k += kReduction;
            depth--;
            boostFactor += boostReduction;
            break;
        case LEAF:
            //1B. If trunk width is past a threshold then create a leaf line
            //1B1.On circle put 4 oval leaves with ragedness at each 90 degrees   and store in trunkV and trunkC  and store in leafV and leafC and create leafI
            //1B2.On circle up one put 4 oval leaves with ragedness at each 90 degrees   and store in trunkV and trunkC and create trunkI  and store in leafV and leafC and create leafI
            //1B3. Repeat until max length
            leafBranch(trunkDiameter, seed, lineHeight);
            //rotate my combined of predesecors
            //rotate based on the previous and current angles
            angleX += ag->angleX;
            angleY = angleY;
            angleZ += ag->angleZ;

            //and add to leaf index with combined
            combinedStartIndices.push_back({ (int)combinedVertices.size() - 1,(int)angleX,(int)angleY,(int)angleZ });

            //add to grouping at depth
            agNew = new AttatchmentGroupings(combinedStartIndices.at((int)combinedStartIndices.size() - 2).at(0),
                                             (int)combinedVertices.size() - 1, (int)angleX, (int)angleY, (int)angleZ, 'L', tag);
            if (tag == 'R') ag->ag[1] = agNew;
            else			ag->ag[0] = agNew;
            //2. Translate and rotate into given location
            //get branch end, get leaf segment start get leaf segment end

            generateTreeCI(END_TRUNK, trunkDiameter, seed, angleX, angleY, angleZ, tag, ag, lineHeight);
            break;
        default:
            return; break;
    }
    //AFTER FUNCTION INSIDE CONSTRUCTOR
    //3. Merge Trunk V with Leaf V into combined V
    //4. Generate rendering properties.
}

float TreeClusterItem::trunk(const float& trunkDiameter, const float& seed, float lineHeight) {
    const float lineMax = lineMAX(trunkDiameter, k);
    TrunkC tC(&combinedVertices, &combinedUV, &combinedNormals, &combinedIndices, lineMax, 530.0f/800.0);
    return tC.buildAllComponents(trunkDiameter, seed, lineHeight);
}

void TreeClusterItem::leafBranch(const float& trunkDiameter, const float& seed, float lineHeight) {
    const float lineMax = lineMAX(trunkDiameter, k);

    LeafContainerC lcC(&combinedVertices, &combinedUV, &combinedNormals, &combinedIndices, lineMax, 530.0f/800.0);
    lcC.buildAllComponenets(trunkDiameter, widthCutoff,  seed, lineHeight);
}
void TreeClusterItem::initiateMove(AttatchmentGroupings* ag){
     int start = ag->start + 1;
    int max = ag->end + 1;
    for (int k = start; k < max; k++) {
        combinedVertices.at(k)  = makeRotations(glm::radians((float)ag->angleX), glm::radians(0.0f),
                                                 glm::radians((float)ag->angleZ), combinedVertices.at(k));
    }
    moveSegments(0, ag);
    return;
}

void TreeClusterItem::moveSegments(int rotationPoint, AttatchmentGroupings* ag){
    for (int m = 0; m < 2; m++) {
        if (ag->ag[m] == nullptr) continue;

        int moveTo = 0;
        int moveFrom = 0;

        const int circularPoints = ag->ag[m]->type == 'L' ? leafPoints : trunkPoints;
        if (ag->ag[m]->side == 'L') {
            moveTo = (ag->end - circularPoints + 1) + 2;
            moveFrom = (ag->ag[m]->start + 1) + 2;
        }
        else {
            moveTo = (ag->end - circularPoints + 1);
            moveFrom = (ag->ag[m]->start + 1) ;
        }

        if (ag->ag[m]->type == 'B') {
            const int start = ag->ag[m]->start + 1;
            const  int max = ag->ag[m]->end + 1;
            for (int k = start; k < max; k++) {
                combinedVertices.at(k) = makeRotations(glm::radians(
                        (float)ag->ag[m]->angleX), -glm::radians(0.0f), glm::radians((float)ag->ag[m]->angleZ),
                                                        combinedVertices.at(k));
            }
        }
        else {
            const int start = ag->ag[m]->start + 1;
            const int max = ag->ag[m]->end + 1;
            for (int k = start; k < max; k++) {
                combinedVertices.at(k) = makeRotations(glm::radians((float)ag->ag[m]->angleX), -glm::radians(0.0f),
                                                       glm::radians((float)ag->ag[m]->angleZ),
                                                        combinedVertices.at(k));
            }
        }

        //translate components onto branch(destination - position)
        glm::vec3 translation = combinedVertices.at(moveTo) - combinedVertices.at(moveFrom);
        glm::vec3 boost = glm::vec3();
        if(trunkDiameter > this->zeroSize){
            boost = boostSegment(ag,ag->ag[m], &combinedVertices);
        }

        if (ag->ag[m]->type == 'B') {
            int start = ag->ag[m]->start + 1;
            int max = ag->ag[m]->end + 1;
            for (int k = start; k < max; k++) {
                combinedVertices.at(k) += translation + boost;
            }
        }
        else {
            int start = ag->ag[m]->start + 1;
            int max = ag->ag[m]->end + 1;
            for (int k = start; k < max; k++) {
                combinedVertices.at(k) += translation + boost;
            }
        }
        connectSegments(ag, m);
        moveSegments(0, ag->ag[m]);
    }
    return;
}

void TreeClusterItem::connectSegments(AttatchmentGroupings* ag, const int& m){
    depth--;
    std::vector<glm::vec3> surfaceNormals;
    //go through each point on upper part of at depth circle
    for (int j = 0; j < trunkPoints; j++) {
            //WORKS
            const GLuint A = ag->end - trunkPoints+ 1 + j;
            const GLuint B = (ag->end - trunkPoints+ 1)+ (j+1) % trunkPoints ;
            const GLuint C = (ag->ag[m]->start+1) + (j+1)% trunkPoints ;
            combinedIndices.push_back(A);
            combinedIndices.push_back(B);
            combinedIndices.push_back(C);
            const GLuint D = C;
            const GLuint E = (ag->ag[m]->start+1)  + j;
            const GLuint F = A;
            combinedIndices.push_back(D);
            combinedIndices.push_back(E);
            combinedIndices.push_back(F);

        // calculate normals
        //------------------

        //step 1: find the surface normal
        glm::vec3 AB = combinedVertices.at(B) -
                       combinedVertices.at(A);
        glm::vec3 AE = combinedVertices.at(E) -
                       combinedVertices.at(A);

        glm::vec3 surfaceNormal = -glm::cross(AB, AE);
        surfaceNormals.push_back(surfaceNormal);
    }
    //step 2: find the average of the surface normals of the surfaces this vertex is part of
    int len =  surfaceNormals.size();
    for(GLuint i = 0; i < len ; i++){
        int iPos = (ag->end - trunkPoints+ 1) + i + 1;
        if(iPos % trunkPoints == 0) iPos =  (ag->end - trunkPoints+ 1) + 0 ;
        combinedNormals.at(iPos) = -glm::normalize(
                surfaceNormals.at(i) + surfaceNormals.at((i+1) % len)
        );
    }

}

void TreeClusterItem::bufferObject(const GLuint& shader_program) {
    if(isAlien)  this->vao = initVertexArray( combinedVertices, combinedIndices, combinedNormals, &vbo, &ebo, &nbo);
    else         this->vao = initVertexArray( combinedVertices, combinedIndices, combinedNormals,
                                              combinedUV, &vbo, &ebo,  &uvbo);
}

void TreeClusterItem::setLocationFromCenter(const float& circleAngle, const float& distanceFromCenter){
    xPos = cos(glm::radians(circleAngle)) * distanceFromCenter;
    zPos =  sin(glm::radians(circleAngle)) * distanceFromCenter;
}
void TreeClusterItem::setLocationWithPoints(const float& xPos, const float& zPos){
    this->xPos = xPos;
    this->zPos = zPos;
}

float TreeClusterItem::getTrunkDiameter(){
    return trunkDiameter;
}

GLuint TreeClusterItem::getTextureId()
{
    static GLuint tC_texture = loadTexture( textureMap,
            GL_NEAREST,
            GL_LINEAR
    );
    return tC_texture;
}
const int TreeClusterItem::getColorType() {
    return colorType;
}

