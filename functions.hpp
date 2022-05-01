#define FUNCTIONS_HPP
const double inf = 1e10;
const double zero = 1e-10;
#define sigma(x) (x<zero||x>-zero?inf:0)
#define u(x,x0) (x<x0?0:1)
#include<math.h>
bool calcfuc(double x,double& y,double value,double a,int rank){
    if(rank<-1){
        return -1;
    }
    if(rank==-1){
        if(x==a){
            y=inf;
        }else{
            y=0;
        }
        return 0;
    }
    double k=1;
    for(int i=1;i<=rank;i++){
        k=k*i;
    }
    k=1/k;
    if(x>=a){
        y=k*pow((x-a),rank);
    }
    else{
        y=0;
    }
    return 0;
}
