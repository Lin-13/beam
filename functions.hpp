#define FUNCTIONS_HPP
const double inf = 1e10;
const double zero = 1e-10;
#define sigma(x) (x<zero||x>-zero?inf:0)
#define u(x,x0) (x<x0?0:1)
#include<math.h>
//function=k*<x-a>^n
bool calcfuncValue(double x,double& y,double k,double a,int n){
    if(n<-1){
        return -1;
    }
    if(n==-1){
        if(x==a){
            y=inf;
        }else{
            y=0;
        }
        return 0;
    }
    if(n==0){
        return k;
    }
    if(x>=a){
        y=k*pow((x-a),n);
    }
    else{
        y=0;
    }
    return 0;
}
//function=k*<x-a>^n
struct funcInfo{
    double k;
    double a;
    int n;
};
//calc function=k*<x-a>^n
std::vector<funcInfo> calcThetaFuncInfo(load l){
    std::vector<funcInfo> funcs;
    funcInfo info1,info2;
    if(l.getRank()==load::q){
        info1.k=1.0/6*l.getConstantValue();
        info2.k=-1.0/6*l.getConstantValue();
        info1.a=l.getSectionValue()[0];
        info2.a=l.getSectionValue()[1];
        info1.n=3;
        info2.n=3;
        funcs.push_back(info1);
        funcs.push_back(info2);
        return funcs;
    }
    if(l.getRank()==load::F){
        info1.k=l.getConstantValue();
        info1.a=l.getSectionValue()[0];
        info1.n=1;
        funcs.push_back(info1);
        return funcs;
    }
    if(l.getRank()==load::M){
        info1.k=l.getConstantValue();
        info1.a=l.getSectionValue()[0];
        info1.n=0;
        funcs.push_back(info1);
        return funcs;
    }
}
std::vector<funcInfo> calcBendFuncInfo(load l){
    std::vector<funcInfo> funcs;
    funcInfo info1,info2;
    if(l.getRank()==load::q){
        info1.k=1.0/24*l.getConstantValue();
        info2.k=-1.0/24*l.getConstantValue();
        info1.a=l.getSectionValue()[0];
        info2.a=l.getSectionValue()[1];
        info1.n=4;
        info2.n=4;
        funcs.push_back(info1);
        funcs.push_back(info2);
        return funcs;
    }
    if(l.getRank()==load::F){
        info1.k=1.0/2*l.getConstantValue();
        info1.a=l.getSectionValue()[0];
        info1.n=2;
        funcs.push_back(info1);
        return funcs;
    }
    if(l.getRank()==load::M){
        info1.k=l.getConstantValue();
        info1.a=l.getSectionValue()[0];
        info1.n=1;
        funcs.push_back(info1);
        return funcs;
    }
}
std::vector<funcInfo> calcThetaFuncInfo(restrain r){
    std::vector<funcInfo> funcs;
    funcInfo info1,info2;
    if(r.getRank()==restrain::M){
        info1.k=1*r.getFValue();
        info1.a=r.getPoint();
        info1.n=1;
        info2.k=1*r.getMValue();
        info2.a=r.getPoint();
        info2.n=0;
        funcs.push_back(info1);
        funcs.push_back(info2);
        return funcs;
    }
    if(r.getRank()==restrain::F){
        info1.k=1*r.getFValue();
        info1.a=r.getPoint();
        info1.n=1;
        funcs.push_back(info1);
        return funcs;
    }
    return funcs;
}
std::vector<funcInfo> calcBendFuncInfo(restrain r){
    std::vector<funcInfo> funcs;
    funcInfo info1,info2;
    if(r.getRank()==restrain::M){
        info1.k=1.0/2*r.getFValue();
        info1.a=r.getPoint();
        info1.n=2;
        info2.k=1*r.getMValue();
        info2.a=r.getPoint();
        info2.n=1;
        funcs.push_back(info1);
        funcs.push_back(info2);
        return funcs;
    }
    if(r.getRank()==restrain::F){
        info1.k=1.0/2*r.getFValue();
        info1.a=r.getPoint();
        info1.n=2;
        funcs.push_back(info1);
        return funcs;
    }
    return funcs;
}
void printfunc(funcInfo fun,const char* s){
    if(fun.k>=0){
        printf("+");
    }
    printf("%.5f*<x-%.2f>^%d%s",fun.k,fun.a,fun.n,s);
}