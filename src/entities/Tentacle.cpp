#include <cmath>
#include <src/loadTexture.hpp>

#include "Trees/Tree.hpp"
#include "Trees/TreeA.hpp"
#include "Trees/TrunkAB.hpp"
#include "Tentacle.hpp"

constexpr int Tentacle::branches;
constexpr int Tentacle::k;
constexpr int Tentacle::previousRotationCap;

constexpr double Tentacle::trunkRatio;
constexpr double Tentacle::branchRatio;

Tentacle::Tentacle(const GLuint& shader_program, Entity* entity, float trunkDiameter, const int& seed,
                   bool isAlien, bool isTextured):
        Tree(heightChunking, boostFactor, seed, shader_program, entity, 'A'){


        this->isAlien = isAlien;
        this->isTextured = isTextured;

    if(isAlien) Tentacle::colorType = COLOR_TREE;
    else Tentacle::colorType = COLOR_LIGHTING;


    textureMap = textureMap1;

	
	this->setMaterial(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.5f), 20.0f);
    treeSetup(shader_program, trunkDiameter, seed);

};

void Tentacle::treeSetup(const GLuint& shader_program, float trunkDiameter, const int& seed){
    draw_mode = GL_TRIANGLES;
    if (trunkDiameter <= 0.0) trunkDiameter = 1.0;
    widthCut = 0.5;//trunkDiameter / 32;
    finalCut = widthCut;

    combinedStartIndices.push_back({-1,0,0,0});
    generateTentacle(0, trunkDiameter, seed, 0, 0, 0, 'C', nullptr, 0);
    rotate(TreeRandom::treeRandom(trunkDiameter, seed,100), glm::vec3(0.0f,1.0f,0.0f));
    bufferObject(shader_program);
}

void Tentacle::generateTentacle(const int& _case, float trunkDiameter, const float& seed,
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
                TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;
                generateTentacle(TRUNK, ShootDiameterBranch / (branches), seed, -std::abs(angleX), angleY, std::abs(angleZ), 'R', agNew, 0);
                TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;
            }
  
            initiateMove(agNew);
            agNew->selfErase();
            delete agNew;
            break;
        case TRUNK:
            if (trunkDiameter < widthCut) {
                generateTentacle(LEAF, trunkDiameter, seed, angleX, angleY, angleZ, tag, ag, 0);
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
                    TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;
                    generateTentacle(TRUNK, ShootDiameterBranch, seed, -std::abs(angleX), angleY, std::abs(angleZ), 'R', agNew, 0);
                    TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;
                }




            }
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

            generateTentacle(END_TRUNK, trunkDiameter, seed, angleX, angleY, angleZ, tag, ag, lineHeight);
            break;
        default:
            return; break;
    }
    //AFTER FUNCTION INSIDE CONSTRUCTOR
    //3. Merge Trunk V with Leaf V into combined V
    //4. Generate rendering properties.
}

float Tentacle::trunk(float trunkDiameter, const float& seed, float lineHeight) {
    const int lineMax = lineMAX(trunkDiameter, k);
    bool loopInitialTrunk;
    const float lineSegments = ((float)lineMax) / heightChunking;
    TrunkAB trunk(&combinedVertices, &combinedUV,
                 seed
    );
    do {
        loopInitialTrunk = trunk.buildTrunk(trunkDiameter, lineSegments);
        TrunkAB::constructionFlowCounter = !TrunkAB::constructionFlowCounter;
    } while (loopInitialTrunk && trunk.getLineHeight() < lineMax);
    if (lineHeight >= lineMax)
        return -1;
    else if(!loopInitialTrunk)
        return lineHeight;
    else return -1;

}

void Tentacle::leafBranch(float trunkDiameter, const float& seed, float lineHeight) {
    const int lineMax = lineMAX(trunkDiameter, k);
    LeafContainerAB lc(&combinedVertices,
                      &combinedIndices,
                       &combinedUV,
                      seed);
    //a leaf container is an object that holds a set of leaves and a branch that they're held on
    lc.buildContainer(trunkDiameter, seed, lineHeight, lineMax);
}

void Tentacle::initiateMove(AttatchmentGroupings* ag){
    glm::mat4 rotation;
    const int circularPoints = TrunkAB::trunkPoints;
    int rotationPoint = std::abs((ag->angleY) % (int)(circularPoints / limiter ));

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

void Tentacle::moveSegments(const int& previousRotation, AttatchmentGroupings* ag) {
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
void Tentacle::bufferObject(const GLuint& shader_program) {
    if(isTextured)  this->vao = initVertexArray( combinedVertices, combinedIndices, combinedNormals, combinedUV,  &vbo, &ebo, &nbo, &uvbo);
    else            this->vao = initVertexArray( combinedVertices, combinedIndices, combinedNormals,  &vbo, &ebo, &nbo);
    //stbi_image_free(image_data);
}

void Tentacle::setTreeLoaded(bool val){
    treeLoaded = val;
}

void Tentacle::setTreeInit(bool val){
    treeInit = val;
}

GLuint Tentacle::getTextureId()
{
    static GLuint tA_texture = loadTexture(
                textureMap,
                GL_LINEAR_MIPMAP_LINEAR,
                GL_NEAREST
        );
    return tA_texture;
}

const int Tentacle::getColorType() {
    return colorType;
}