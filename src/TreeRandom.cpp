#include "TreeRandom.hpp"
#include <cmath>
#include <cstdio>
int TreeRandom::k = 777;

int TreeRandom::treeRandom(float trunkDiameter, float seed, float lineHeight) {
    int randomSeedValue = ((int)(trunkDiameter * seed * (((int)lineHeight % 10 * (int)(lineHeight*7) % 100 + 1 ) * 10) * 3)) % (k*10 * (int)ceil(trunkDiameter));
    if (randomSeedValue % 2 == 0) randomSeedValue = -(randomSeedValue + 1);
    return randomSeedValue;
}
int TreeRandom::treeOddEvenRandom(float trunkDiameter, float seed, float lineHeight) {
    int randomSeedValue = ((int)(trunkDiameter * seed * (((int)lineHeight % 10 * (int)(lineHeight*7) % 100 + 1 ) * 10) * 3)) % (k*10 * (int)ceil(trunkDiameter+1));
    return randomSeedValue;
}
int TreeRandom::branchAngleFromRandom(float trunkDiameter, float seed, float lineHeight,
                                      int maxYBranchAngle , int minYBranchAngle) {
    return treeRandom(trunkDiameter, seed, lineHeight) % (maxYBranchAngle - minYBranchAngle) + minYBranchAngle;
}
int TreeRandom::trunkAngleFromRandom(float trunkDiameter, float seed, float lineHeight,
                                     int maxYTrunkAngle ,int minYTrunkAngle) {
    return treeRandom(trunkDiameter, seed, lineHeight) % (maxYTrunkAngle - minYTrunkAngle) + minYTrunkAngle;
}

#include <iostream>
unsigned long TreeRandom::middleSquareRange(int seed, float max, float min){
    long _max = max, _min = min, _seed = pow(seed, 2);
//    std::cout << "Rand " <<  (unsigned long)(_seed) % (int)(_max - _min) + _min << " " << _max << " "<< _min << "\n";
    long modulo = (_max * 100 - _min * 100);
    unsigned long rand  = (unsigned long)(_seed) % (int)( modulo == 0 ? 991 : modulo) + _min * 100;
    return rand / 100;
}