////////DISCONTINUED////////////

#include <cmath>

#include "TreeDistributor.hpp"

int TreeDistributor::seed;
bool TreeDistributor::ready;
GLuint voidVal = 0;
GLuint& TreeDistributor::shader_programA = voidVal, TreeDistributor::shader_programB= voidVal,
        TreeDistributor::shader_programC= voidVal;
Entity* TreeDistributor::entity;

void TreeDistributor::init(GLuint& shader_programA,GLuint&  shader_programB,GLuint&  shader_programC,
                 Entity* entity
){
    TreeDistributor::ready = true;
    TreeDistributor::shader_programA = shader_programA;
    TreeDistributor::shader_programB = shader_programB;
    TreeDistributor::shader_programC = shader_programC;
    if(entity == nullptr) TreeDistributor::ready = false;
    else TreeDistributor::entity = entity;
}

void TreeDistributor::setEntity(Entity* entity){
    TreeDistributor::ready = true;
    if(entity == nullptr) TreeDistributor::ready = false;
    else TreeDistributor::entity = entity;
}

Tree* TreeDistributor::setTreeType(int seed, float width){
    try{
        if(!ready) throw(-1);
    }
    catch(int e){
        std::cerr << "DISTRIBUTOR NOT INITIALIZED OR ENITITY WAS NULLPTR"
                " -- CALL\"static void init(GLuint& shader_programA,GLuint&  "
                "shader_programB,GLuint&  shader_programC, Entity* entity){\" OR"
                "\"  static void setEntity(Entity* entity){ \"";
    }
    TreeDistributor::seed = std::abs(seed);
    std::cout << "SEED" << TreeDistributor::seed % 10  << "\n";
    if(TreeDistributor::seed % 10 < 2){
        TreeA* tA = new TreeA(shader_programA, entity, width*3, TreeDistributor::seed);
        return tA;
    }
    else if(TreeDistributor::seed % 10 < 7){
        TreeB* tB = new TreeB(shader_programB, entity, width, TreeDistributor::seed);
        return tB;
    }
    else{
        TreeC* tc = new TreeC(0 + TreeDistributor::seed % 15, shader_programC, entity, width*0.5, TreeDistributor::seed);
        return tc;
    }
}