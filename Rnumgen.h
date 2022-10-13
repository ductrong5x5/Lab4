/*  Author: Duc Trong Nguyen
    Net ID: dnguye38
    Description: This file define class rnumgen, which will be used in pertub pixel list.
*/
#ifndef __RNUMGEN_H__
#define __RNUMGEN_H__

#include <functional>
#include <vector>
#include <numeric>
#include <algorithm>
using namespace std;

class rnumgen {
  public:
    rnumgen(int i , vector<int> &v);  //create probability list of each 15bit number
    int rand()const;                  //create random
    void randperm (vector<int> &v);   //create 30bit number and do random permutation
  private:
    vector<int> F;                  //probability of 15 bit number
};

#endif
