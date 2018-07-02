#include <cmath>
#include <src/loadTexture.hpp>

#include "Tree.hpp"
#include "TreeA.hpp"

constexpr int TreeA::branches;
constexpr int TreeA::k;
constexpr int TreeA::previousRotationCap;

constexpr double TreeA::trunkRatio;
constexpr double TreeA::branchRatio;

TreeA::TreeA(const GLuint& shader_program, Entity* entity, float trunkDiameter, const int& seed, bool isAlien):
        Tree(heightChunking, boostFactor, seed, shader_program, entity, 'A'){

    std::clock_t startTime;
    double duration;
    startTime = std::clock();


    textureMap = textureMap1;

    this->isAlien = isAlien;
    if(isAlien) TreeA::colorType = COLOR_TREE;
    else TreeA::colorType = COLOR_LIGHTING;

    treeSetup(shader_program, trunkDiameter, seed);

};

void TreeA::treeSetup(const GLuint& shader_program, float trunkDiameter, const int& seed){
    draw_mode = GL_TRIANGLES;
    if (trunkDiameter <= 0.0) trunkDiameter = 1.0;
    finalCut = widthCut;

    combinedStartIndices.push_back({-1,0,0,0});
    generateTreeA(0, trunkDiameter, seed, 0, 0, 0, 'C', nullptr, 0);
    rotate(TreeRandom::treeRandom(trunkDiameter, seed,100), glm::vec3(0.0f,1.0f,0.0f));
    bufferObject(shader_program);
}

void TreeA::generateTreeA(const int& _case, float trunkDiameter, const float& seed,
                          float angleX, float angleY, float angleZ, char tag,
                          AttatchmentGroupings* ag, float lineHeight) {
    int currentLineLength = lineHeight;
    float ShootDiameterBranch = 0;
    float ShootDiameterTrunk = 0;
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
                                             (int)combinedVertices.size() - 1,		//TYPE //SIDE
                                             (int)angleX, (int)angleY, (int)angleZ, 'B', 'C');

            //1A5. Start N new recursive functions from seed based angle at a certain base position
            ShootDiameterBranch = shootCalculation(trunkDiameter, branchRatio, branches);
            ShootDiameterTrunk = shootCalculation(trunkDiameter, trunkRatio, branches);

            //1A6. On new branch create junction and new shoot
            angleY = TreeRandom::treeOddEvenRandom(trunkDiameter, seed* ( 1) * 3, (lineHeight+1)* ( 1));
            for (int n = 0; n < branches; n++) {
                angleZ = TreeRandom::branchAngleFromRandom(trunkDiameter, seed * (n+1), currentLineLength* (n + 1), maxYBranchAngle, minYBranchAngle);
                angleX = TreeRandom::branchAngleFromRandom(trunkDiameter, seed* (n + 1) * 7, currentLineLength* (n + 1), maxYBranchAngle, minYBranchAngle); //* (((int)seed) % 2 == 0 ? -1 : 1);
                //angleY = angleY;

                generateTreeA(TRUNK, ShootDiameterBranch / (branches), seed, -std::abs(angleX), angleY, std::abs(angleZ), 'R', agNew, 0);
            }
            //1A7. On new trunk join to junction and continue
            angleZ = TreeRandom::trunkAngleFromRandom(trunkDiameter, seed* 71, currentLineLength, maxYTrunkAngle, minYTrunkAngle) ;
            angleX = TreeRandom::trunkAngleFromRandom(trunkDiameter, seed * 9, currentLineLength, maxYTrunkAngle, minYTrunkAngle); //* (((int)seed) % 2 == 0 ? 1 : -1);
            ///angleY = angleY;
            TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;
            generateTreeA(TRUNK, ShootDiameterTrunk, seed, std::abs(angleX), angleY, -std::abs(angleZ), 'L', agNew, currentLineLength);
            TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;
            initiateMove(agNew);
            agNew->selfErase();
            delete agNew;
            break;
        case TRUNK:
            if (trunkDiameter < widthCut) {
                generateTreeA(LEAF, trunkDiameter, seed, angleX, angleY, angleZ, tag, ag, 0);
                if (trunkDiameter < finalCut) {
                    return;
                }
            }
            else{
                currentLineLength = trunk(trunkDiameter, seed, lineHeight);
            }

            //rotate based on the previous and current angles
            angleX += ag->angleX;
            ///angleY = angleY;
            angleZ += ag->angleZ;

            //add the sum of angles onto the current branch
            combinedStartIndices.push_back({ (int)combinedVertices.size() - 1, (int)angleX, (int)angleY, (int)angleZ });

            //store current branch poosition and rotation sum at depth
            agNew = new AttatchmentGroupings(combinedStartIndices.at((int)combinedStartIndices.size() - 2).at(0),
                                             (int)combinedVertices.size() - 1, (int)angleX, (int)angleY, (int)angleZ, 'B', tag);
            if (tag == 'R') ag->ag[1] = agNew;
            else			ag->ag[0] = agNew;

            //1A5. Start N new recursive functions from seed based angle at a certain base position
            angleY = TreeRandom::treeOddEvenRandom(trunkDiameter, seed * 5, lineHeight);
            if (lineHeight == -1 || currentLineLength == -1) {
                ShootDiameterTrunk = shootCalculation(trunkDiameter, trunkRatio, branches);
                currentLineLength = 0;
            }
            else{
                ShootDiameterBranch = shootCalculation(trunkDiameter, branchRatio, branches);
                ShootDiameterTrunk = shootCalculation(trunkDiameter, trunkRatio, branches);
                for (int n = 0; n < branches; n++) {
                    //1A6. On new branch create circle then indices flowing back once to center cirlce.
                    angleZ = TreeRandom::branchAngleFromRandom(trunkDiameter, seed, currentLineLength, maxYBranchAngle, minYBranchAngle);
                    angleX = TreeRandom::branchAngleFromRandom(trunkDiameter, seed * 7, currentLineLength, maxYBranchAngle, minYBranchAngle) * (((int)seed) % 2 == 0 ? -1 : 1);;
                    ///angleY = angleY;
                    generateTreeA(TRUNK, ShootDiameterBranch, seed, -std::abs(angleX), angleY, std::abs(angleZ), 'R', agNew, 0);
                }
            }

            //1A7. On new trunk create circle then indices flowing back once to top circle.
            angleZ = TreeRandom::trunkAngleFromRandom(trunkDiameter, seed, currentLineLength, minYTrunkAngle, maxYTrunkAngle);
            angleX = TreeRandom::trunkAngleFromRandom(trunkDiameter, seed * 7, currentLineLength, minYTrunkAngle, maxYTrunkAngle) * (((int)seed) % 2 == 0 ? -1 : 1);;
            angleY = angleY;
            TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;
            generateTreeA(TRUNK, ShootDiameterTrunk, seed, std::abs(angleX), angleY, -std::abs(angleZ), 'L', agNew, currentLineLength);
            TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;
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
            ///angleY = angleY;
            angleZ += ag->angleZ;

            //and add to leaf index with combined
            combinedStartIndices.push_back({ (int)combinedVertices.size() - 1,(int)angleX,(int)angleY,(int)angleZ });

            //add to grouping at depth
            agNew = new AttatchmentGroupings(combinedStartIndices.at((int)combinedStartIndices.size() - 2).at(0),
                                             (int)combinedVertices.size() - 1,
                                             (int)angleX, (int)angleY, (int)angleZ, 'L', tag);
            if (tag == 'R') ag->ag[1] = agNew;
            else			ag->ag[0] = agNew;
            //2. Translate and rotate into given location
            //get branch end, get leaf segment start get leaf segment end

            generateTreeA(END_TRUNK, trunkDiameter, seed, angleX, angleY, angleZ, tag, ag, lineHeight);
            break;
        default:
            return; break;
    }
    //AFTER FUNCTION INSIDE CONSTRUCTOR
    //3. Merge Trunk V with Leaf V into combined V
    //4. Generate rendering properties.
}

float TreeA::trunk(float trunkDiameter, const float& seed, float lineHeight) {
    const int lineMax = lineMAX(trunkDiameter, k);
    bool loopInitialTrunk;
    const float lineSegments = ((float)lineMax) / heightChunking;
    TrunkAB trunk(&combinedVertices, &combinedUV,
                 seed
    );
    do {
        loopInitialTrunk = trunk.buildTrunk(trunkDiameter, lineSegments);
    } while (loopInitialTrunk && trunk.getLineHeight() < lineMax);
    if (lineHeight >= lineMax)
        return -1;
    else if(!loopInitialTrunk)
        return lineHeight;
    else return -1;

}

void TreeA::leafBranch(float trunkDiameter, const float& seed, float lineHeight) {
    const int lineMax = lineMAX(trunkDiameter, k);
    LeafContainerAB lc(&combinedVertices,
                       &combinedIndices,
                       &combinedUV,
                      seed);
    //a leaf container is an object that holds a set of leaves and a branch that they're held on
    lc.buildContainer(trunkDiameter, seed, lineHeight, lineMax);
}
//atatchment grouping -> See Tree.hpp
void TreeA::initiateMove(AttatchmentGroupings* ag){
    glm::mat4 rotation;
    const int circularPoints = TrunkAB::trunkPoints;
    int rotationPoint = std::abs((ag->angleY) % (int)(circularPoints / limiter ));

    //limit rotations to one segment at a time
    rotationPoint = rotationPoint == 0 ? 0 : 1;

    const float r = 360.0f/circularPoints  * (rotationPoint);
    const int start = ag->start + 1;
    const int max = ag->end + 1;
    for (int k = start; k < max; k++) {
        combinedVertices.at(k)  = makeRotations(glm::radians((float)ag->angleX), glm::radians(r),
                                                 glm::radians((float)ag->angleZ),
                                                 combinedVertices.at(k));
    }
    const int previousRotation = rotationPoint;
    //create elements for segment
    computeElementsInitial(ag);
    moveSegments(previousRotation, ag);
}



/*
 *
 * 1_______2
 *  |     |
 *  |     |
 *  |_____|
 * 0       3
 *
 * This figure rotates and needs to connect with a lower square.
 *
 * eg.
 *  0______1
 *  |     |
 *  |     |
 *  |_____|
 * 3       2
 *
 * The methods below do this procedure and pass on rotation information to the move connect, compute and recurse back
 * through binary recursion until the furthest branch has been reached.
 *
 */
void TreeA::moveSegments(const int& previousRotation, AttatchmentGroupings* ag) {
    for (int m = 0; m < 2; m++) {
        if (ag->ag[m] == nullptr) continue;
        int moveTo = 0;
        int moveFrom = 0;

        if(previousRotation < previousRotationCap) limiter = 1;
        else if(previousRotation >= previousRotationCap) limiter = 0.01;

        int circularPoints = ag->ag[m]->type == 'L' ? LeafContainerAB::leafBranchPoints : TrunkAB::trunkPoints;
        int rotationPoint = std::abs((ag->ag[m]->angleY) % (int)(circularPoints / limiter ));

        rotationPoint = rotationPoint < 1 ? 1 : 0;

        const int toPnt = (circularPoints-rotationPoint + (previousRotation));
        const int fromPnt = (previousRotation);

        if (ag->ag[m]->side == 'L') {
            moveTo = (ag->end - circularPoints + 1) + (( 0 + toPnt) % circularPoints);
            moveFrom = (ag->ag[m]->start + 1)  + ((0 + fromPnt) % circularPoints);
        }
        else {
            moveTo = (ag->end - circularPoints + 1) + (int)(circularPoints/2.0 + toPnt ) % circularPoints;
            moveFrom = (ag->ag[m]->start + 1) + (int)(circularPoints/2.0  + fromPnt) % circularPoints;
        }

        const float r = 360.0f/circularPoints  * (toPnt);

        const  int start = ag->ag[m]->start + 1;
        const int max = ag->ag[m]->end + 1;

        for (int k = start; k < max; k++) {
            combinedVertices.at(k) = makeRotations( glm::radians((float)ag->ag[m]->angleX), glm::radians(r),
                                                     glm::radians((float)ag->ag[m]->angleZ), combinedVertices.at(k));
        }

        //translate components onto branch(destination - position)
        const glm::vec3 translation = combinedVertices.at(moveTo) - combinedVertices.at(moveFrom);
        //elevate from point
        const glm::vec3 boost = boostSegment(ag, ag->ag[m], &combinedVertices) *  (float)(heightChunking * boostFactor);
        for (int k = start; k < max; k++) {
            combinedVertices.at(k) += translation + boost;
        }
        //create the connector's elements from previous to m
        connectSegments(ag, m,toPnt, fromPnt, circularPoints, &combinedIndices);
        //create elements for segment
        computeElementsInitial(ag->ag[m]);
        //move them to position
        moveSegments(toPnt, ag->ag[m]);
    }
    return;
}

//PUT TEXTURE LOADING IN SEPERATE CLASS. MAKE IT ONLY CALLED ONCE FOR THE FIRST TREE LOADED.
void TreeA::bufferObject(const GLuint& shader_program) {
    if(isAlien)  this->vao  = initVertexArray(combinedVertices, combinedIndices, combinedNormals,
                                              &vbo, &ebo, &nbo);
    else         this->vao = initVertexArray(combinedVertices, combinedIndices, combinedNormals,
                                             combinedUV,  &vbo, &ebo, &nbo, &uvbo);
}

//use carlo's loading systems
GLuint TreeA::getTextureId()
{
    static GLuint tA_texture = loadTexture(
                textureMap,
                GL_NEAREST,
                GL_LINEAR
        );
    return tA_texture;
}

const int TreeA::getColorType() {
    return colorType;

}