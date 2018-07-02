#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <cmath>
#include <glm/glm.hpp>

#include "src/entities/Entity.hpp"
#include "src/entities/DrawableEntity.hpp"
#include "Tree.hpp"


Tree::Tree(int heightChunking, float boostFactor, float seed, const GLuint& shader_program, Entity* entity, const char& type)
        : DrawableEntity(shader_program, entity){
    this->seed = seed;
    draw_mode = GL_TRIANGLES;
    this->heightChunking = heightChunking;
    this->boostFactor = boostFactor;
    this->type = type;

    this->setMaterial(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.5f), 0.5f);
}

Tree::~Tree() {
//    delete combinedVertices;
//    delete combinedIndices;
//    delete combinedNormals;
//    delete combinedUV;
//    delete combinedStartIndices;
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &nbo);
    glDeleteBuffers(1, &uvbo);
   glDeleteVertexArrays(1, &vao);
}

glm::vec3 Tree::boostSegment(const AttatchmentGroupings* agLow,const AttatchmentGroupings* agHigh,
                             const std::vector<glm::vec3>* vPntr){
    glm::vec3 AB = vPntr->at(agLow->end) - vPntr->at(agLow->end -1);
    glm::vec3 AC = vPntr->at(agLow->end-2) - vPntr->at(agLow->end -1);
    glm::vec3 low =  glm::normalize(glm::cross(AB, AC));
    glm::vec3 KJ = vPntr->at(agHigh->start + 1) - vPntr->at(agHigh->start+2);
    glm::vec3 KL = vPntr->at(agHigh->start + 3) - vPntr->at(agHigh->start+2);
    glm::vec3 high =  -glm::normalize(glm::cross(KJ, KL));
    return low + high;
}

//values in radians
glm::vec3 Tree::makeRotations(const float& xRot, const float& yRot, const float& zRot, glm::vec3 vector){
    //ROTATION ABOUT X
    vector = glm::vec3(
            vector.x
            , vector.y * cos(xRot) - vector.z * sin(xRot)
            ,	vector.y * sin(xRot) + vector.z * cos(xRot));
    //ROTATION ABOUT Z
    vector = glm::vec3(
            vector.x * cos(zRot) - vector.y * sin(zRot)
            , vector.x * sin(zRot) + vector.y * cos(zRot)
            ,	vector.z );
    //ROTATION ABOUT Y
    vector = glm::vec3(
            vector.x * cos(yRot) + vector.z * sin(yRot)
            , vector.y
            ,	-(vector.x * sin(yRot)) + vector.z * cos(yRot));

    return vector;
}

void Tree::connectSegments(const AttatchmentGroupings* ag, const int& m,
                           const int& rotPoint, const int& prevPoint, const int& circularPoints,
                     std::vector<GLuint>* indPntr){
    int set = std::abs((circularPoints - rotPoint) + prevPoint);
    TrunkAB::buildConnectorElements(ag->end - TrunkAB::trunkPoints + 1, ag->ag[m]->start + 1, set, ag->side,
                                   &combinedIndices, &combinedVertices, &combinedNormals);
}

void Tree::computeElementsInitial(const AttatchmentGroupings* ag) {
    if(ag->type == 'B'){
        TrunkAB::buildTrunkElements(ag->start + 1, ag->end,
                                    &combinedIndices, &combinedVertices, &combinedNormals);
    }
    else
        LeafContainerAB::buildLeafContainerElements(ag->start + 1, ag->end,
                                                    &combinedIndices, &combinedVertices, &combinedUV, &combinedNormals);
}

float Tree::shootCalculation(const float& trunkDiameter, const double& ratio, const int& branches){
    return pow(pow(trunkDiameter, 2) / (branches + 1), 1.0 / 2.0) * ratio;
}
int Tree::lineMAX(const float& trunkDiameter, int k) {
    return ceil(pow(pow(trunkDiameter, 2) * k, 1.0 / 2.0));
}


const std::vector<glm::vec3>& Tree::getVertices() const {
    return combinedVertices;
}

GLuint Tree::getVAO() {
    return this->vao;
}

const int Tree::getColorType() {
    return COLOR_TREE;
}
