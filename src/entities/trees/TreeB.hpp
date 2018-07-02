/*
____DOCUMENTATION_OF_TREE_GENERATION_____

ONE CENTRAL .CPP FILE CONTROLS GENERATION OF TREE STRUCTURE. IS BROKEN DOWN INTO A LOG COMPONENT AND A LEAF BRANCH COMPONENT THAT
ARE MESHED TOGETHER USING A RECURISIVE ALGORITHM STORE COMBINED VERTICES(THROUGH POINTERS)
	TreeA(const GLuint& shader_program, float trunkDiameter, float seed)
CALL TO GENERATE OBJECT WILL RESULT IN AN OBJECT WITH A STORED VAO IN THESE PARAMETERS.
CREATES TRUNK AND LEAF VECTORS+INDICES WHICH ARE MERGED TOGETHER ARE SENT TO THE GPU.

CURRENT LOADING SPEED SUGESTS THE 4-8 DIAMETER RANGE WILL GENERATE QUICKLY. SCALING TO HEIGHTS MAY BE NESCICARY.

PROGRAM ENTRY POINT IN IN THE generateTreeA FUNCTION WHEREIN IT STARTS TO BUILD THE STARTER TRUNK,
THEN ENTERS BINARY RECURSIONS OF ITSELF TO GENERATE THE TRUNKS AND LEAVES WHEREUPON REACHING THE BASE CASE IN MOVES SEGMENTS OF
ONE RECURSION ONTO THE STARTER TRUNK AND PROCEEDS TO DO THE NEXT BRANCH. RECURSIONS SET PARAMETERS OF ANGLES AND SEGMENT WIDTHS
DECREASING AT A PYTHAGOREAN RATE.

TRUNKS HAVE FOUR PROPERTIES.
  1) THE WIDTH OF THE SEGMENT
  2) THE JAGEDNESS(BUMPYNESS) OF THE SEGMENT
  3) THE MAX HEIGHT IT CAN REACH
  4) THE SEED RELATED TO IT
THEY ARE STRAIGHT LINE PIECES OF ROUGHLY CIRCULAR SIZE

LEAF BRANCHES HAVE FOUR PROPERTIES
  1) THE WIDTH OF THE BRANCH SEGMENT
  2) THE JAGEDNESS(BUMPYNESS) OF THE SEGMENT'S LEAVES
  3) THE MAX HEIGHT IT CAN REACH
  4) THE SEED RELATED TO IT
AT EVERY PART OF THE BRANCH IT SPLITS  INTO TWO LEAVES ALTERNATING IN PERPENDICULARS.
AT THE LAST 2 PARTS THE LEAVES GET SMALLER UNTIL THE LAST WHERE IT ENDS WITH A LEAF.
LEAFS ARE DOUBLE SIDED FOR BACKFACE CULLING.

SEGMENTS ARE STORED INTO ATTATCHMENT GROUPINGS AS A LINKED LIST AND ARE STICHED TOGETHER IN THE MOVE PROCESS USING THE
ANGLES COMPUTED DURING RECURSIONS
*/

#ifndef COMP371_TREE_B_HPP
#define COMP371_TREE_B_HPP

#define START_TRUNK 0
#define TRUNK 2
#define LEAF 3
#define END_TRUNK -1


#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h> // include GL Extension Wrangler
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include <iostream>
#include <cstdio>
#include <ctime>

#include "src/TreeRandom.hpp"
#include "TrunkAB.hpp"
#include "LeafContainerAB.hpp"

#include "src/entities/Entity.hpp"
#include "src/entities/DrawableEntity.hpp"
#include "Tree.hpp"

class TreeB : public Tree {
private:

    //CONSIDERATION FOR MULTITHREADED LOADING
    bool treeLoaded = false;

    const int branches = 1;
    const int k = 1;
    float widthCut = 0.325;

    static constexpr float boostFactor = 0.52;
    static constexpr int heightChunking = 6;//INVERSE

    const int minYBranchAngle = 30;
    const int maxYBranchAngle = 90;
    const int minYTrunkAngle = 30;
    const int maxYTrunkAngle = 90;

    double trunkRatio = 1.0;
    double branchRatio = 0.850;


    bool isAlien;

    void generateTreeB(const int& _case, float trunkDiameter, const float& seed,
                       float angleX, float angleY, float angleZ, char tag,
                       AttatchmentGroupings* ag, float lineHeight);

    float trunk(float trunkDiameter, const float& seed, float lineHeight);

    void leafBranch(float trunkDiameter, const float& seed, float lineHeight);

    void bufferObject(const GLuint& shader_program);

    void initiateMove(AttatchmentGroupings* ag);

    void moveSegments(const int& previousRotation, AttatchmentGroupings* ag);

    bool treeSetup(const GLuint& shader_program, float trunkDiameter, const int& seed);
protected:
    int colorType;
public:
    const std::vector<glm::vec3>& getVertices() const override;

    GLuint getVAO() override;

    TreeB(const GLuint& shader_program, Entity* entity, double trunkDiameter, int seed, bool isAlien);

    GLuint getTextureId() override;
    const int getColorType() override;
};
#endif //treeA
