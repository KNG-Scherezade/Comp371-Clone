
#ifndef PROCEDURALWORLD_TREE_H
#define PROCEDURALWORLD_TREE_H

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "src/constants.hpp"
#include "TrunkAB.hpp"
#include "LeafContainerAB.hpp"
#include "src/entities/Entity.hpp"
#include "src/entities/DrawableEntity.hpp"

//wrapper class
class Tree: public DrawableEntity {
private:
    char type;
protected:
    class AttatchmentGroupings {
    public:
        //START END ANGLEX ANGLEY ANGLEZ
        int start;
        int end;
        int angleX;
        int angleY;
        int angleZ;
        char type;
        char side;

        AttatchmentGroupings* ag[2];

    AttatchmentGroupings(const int& start,	const int& end,
                         const int& angleX, const int& angleY, const int& angleZ,
                         const char& type,	const char& side){
        this->start = start;
        this->end = end;
        this->angleX = angleX;
        this->angleY = angleY;
        this->angleZ = angleZ;
        this->type = type;
        this->side = side;
        ag[0] = nullptr; ag[1] = nullptr;
    }
    void selfErase() {
        for (AttatchmentGroupings* agI: this->ag){
            if (agI != nullptr) {
                agI->selfErase();
                delete agI;
                agI = nullptr;
            }
        }
    }
};

    float shootCalculation(const float& trunkDiameter, const double& ratio,const int& branches);
    int lineMAX(const float& trunkDiameter, int k);

    std::vector<AttatchmentGroupings> branchStore;
    std::vector<glm::vec3> combinedVertices;
    std::vector<GLuint> combinedIndices;
    std::vector<glm::vec3> combinedNormals;
    std::vector<glm::vec2> combinedUV;
    std::vector<std::vector<int>> combinedStartIndices;
    GLuint vao; GLuint vbo; GLuint ebo; GLuint nbo; GLuint uvbo;

    float heightChunking;
    float boostFactor;
    float seed;

    float widthCut;
    float finalCut;

    Entity* e;

public:

    char getType(){return type;}

    glm::vec3 boostSegment(const AttatchmentGroupings* agLow, const AttatchmentGroupings* agHigh,
                           const std::vector<glm::vec3>* vPntr);

    //values in radians
    glm::vec3 makeRotations(const float& xRot, const float& yRot, const float& zRot, glm::vec3 vector);

    void connectSegments(const AttatchmentGroupings* ag, const int& m,
                         const int& rotPoint, const int& prevPoint, const int& circularPoints,
                         std::vector<GLuint>* indPntr);

    void computeElementsInitial(const AttatchmentGroupings* ag);

    const std::vector<glm::vec3>& getVertices() const override;

    GLuint getVAO() override;

    const int getColorType() override;

    Tree(int heightChunking, float boostFactor, float seed, const GLuint& shader_program, Entity* entity, const char& type);
    ~Tree();
};


#endif //PROCEDURALWORLD_TREE_H
