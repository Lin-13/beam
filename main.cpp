//main.cpp
#include<iostream>
#include<vector>
#include"beam.hpp"
using namespace std;
int main(){
    load l1(1,vector<double>{0.9},load::F);
    restrain r0(1,restrain::M);
    restrain r1(0,restrain::F);
    beam b1;
    b1.addLoad(l1);
    //b1.addRestrain(r1);
    b1.addRestrain(r0);
    b1.setE(1e6);
    b1.setI(1e-6);
    bool b=b1.calcRestrain();
    b1.listLoad();
    b1.listRestrain();
    if(b==true){

        b1.calcBend();
    }else{
        cout<<"wrong"<<endl;
    }
    return 0;
}