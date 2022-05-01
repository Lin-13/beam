#include<iostream>
#include<vector>
#include"beam.hpp"
using namespace std;
int main(){
    load l1(1,vector<double>{0,1},load::q);
    restrain r1(0,restrain::M);
    beam b1;
    b1.addLoad(l1);
    b1.addRestrain(r1);
    b1.calcRestrain();
    b1.listLoad();
    b1.listRestrain();
    return 0;
}