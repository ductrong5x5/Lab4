/*  Author: Duc Trong Nguyen
    Net ID: dnguye38
    Description: This file declare funtions for random generator and random permutation
*/
#include "Rnumgen.h"

using namespace std;
//create probability list of each 15bit number
rnumgen::rnumgen(int seedvalue, vector<int> &v) {
  srand(seedvalue);
  F.resize(v.size());
  partial_sum(v.begin(), v.end(), F.begin());
  transform(F.begin(), F.end(), F.begin(),bind2nd(std::divides<float>(), F.back()));
} 
//create random
int rnumgen::rand()const {
  const double randmax = RAND_MAX+1.0;
  const double p = (double)std::rand()/randmax;
  return upper_bound(F.begin(), F.end(), p) - F.begin();
}
//create 30bit number and do random permutation
void  rnumgen::randperm(vector<int> &v) {
  for (int i=(int)v.size()-1; i>0; --i) {
    int r1;
    int r2;
    r1=rnumgen::rand(); //create first random number
    r2=rnumgen::rand(); //create second random number
    int r3=0x0;
    r3 = r1 << 15;      //add 15bit of first random
    r3 |= r2;           //add 15bit of second random
    swap(v[i], v[r3 % (i+1)]);  //swap for random permutation
  }
} 