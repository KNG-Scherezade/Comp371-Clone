//
// Created by Tof on 2017-11-18.
//

#ifndef PROCEDURALWORLD_TREERANDOM_H
#define PROCEDURALWORLD_TREERANDOM_H

class TreeRandom {
public:
    static int k;

    static int treeRandom(float trunkDiameter, float seed, float lineHeight);
    static int treeOddEvenRandom(float trunkDiameter, float seed, float lineHeight);
    static int branchAngleFromRandom(float trunkDiameter, float seed, float lineHeight, int maxYBranchAngle , int minYBranchAngle);
    static int trunkAngleFromRandom(float trunkDiameter, float seed, float lineHeight, int maxYTrunkAngle ,int minYTrunkAngle);
    static unsigned long middleSquareRange(int seed, float max, float min);
};

#endif //PROCEDURALWORLD_TREERANDOM_H
