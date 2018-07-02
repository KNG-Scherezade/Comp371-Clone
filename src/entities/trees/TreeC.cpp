////////DISCONTINUED////////////

#ifndef COMP371_TREE_C_cPP
#define COMP371_TREE_C_cPP

#include <src/loadTexture.hpp>
#include <src/entities/World.hpp>
#include "TreeC.hpp"

    int TreeC::spacingConstant = 5;

    int TreeC::maxWidth(const float& trunkDiameter){
        return (int)(pow(spacingConstant * (trunkDiameter+1), 1.0/2) + trunkDiameter);
    }

    TreeC::TreeC(int numberOfTrees, const GLuint& shader_program, Entity* entity, float trunkDiameter, long seed, bool isAlien,
                 std::vector<Tree*>& treeContainer, glm::vec3 pos, float magnitude,
                    float min_hitbox_y, float max_hitbox_y, std::vector<HitBox2d>& hbEnt){


        //center piece
        auto * tci = new TreeClusterItem(shader_program, entity, trunkDiameter, seed, isAlien);
        tci->setLocationWithPoints(0, 0);
        tci->setPosition(pos + glm::vec3(tci->xPos, 0, tci->zPos));
        tci->scale(magnitude);

        treeContainer.emplace_back(tci);
        hbEnt.emplace_back(*tci, min_hitbox_y, max_hitbox_y);

        //distribute in random cirlce
        float tempTrunkDiameter;
        int i = 0;
        for (; i < numberOfTrees; i++) {
            if(sizeVariation != 0){
                tempTrunkDiameter = TreeRandom::middleSquareRange(seed, trunkDiameter * 2,  sizeVariation) / 20.0f;
                seed = (unsigned long)(pow(seed, 2) * (tempTrunkDiameter+1)) % 99991;
            }
            else tempTrunkDiameter = trunkDiameter / 1.0f;
            seed = seed == 0 ? (tempTrunkDiameter + 1) * 913 * magnitude : seed;

            float distanceFromCenter = TreeRandom::middleSquareRange(seed, maxWidth(trunkDiameter), 1.0);
            distanceFromCenter = distanceFromCenter <= 1 ? TreeRandom::middleSquareRange(seed * 2, maxWidth(trunkDiameter)* 1.00 , 1.0) : distanceFromCenter;
            float circleAngle = TreeRandom::middleSquareRange(seed, 360.0, 0.0)  * TreeRandom::treeOddEvenRandom(distanceFromCenter, seed, circleAngle);

            float distScale = .125f;
            float xPos = cos(glm::radians(circleAngle)) * distanceFromCenter * distScale;
            float zPos =  sin(glm::radians(circleAngle)) * distanceFromCenter * distScale;

            auto * tci = new TreeClusterItem(shader_program, entity,
                                                       tempTrunkDiameter <= 0? 0:  tempTrunkDiameter,
                                                       seed, isAlien);

            tci->setLocationWithPoints(xPos , zPos);
            tci->setPosition(pos + glm::vec3(tci->xPos + 0.05, 0.0, tci->zPos));
            tci->scale(magnitude);

            treeContainer.emplace_back(tci);
            hbEnt.emplace_back(*tci, min_hitbox_y, max_hitbox_y);
        }
    }


    void TreeC::setSpacingConstant(int k){
        TreeC::spacingConstant = k;
    }


#endif