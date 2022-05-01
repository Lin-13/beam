#define BEAM_HPP
#include<vector>
#include<stdio.h>
#include"load.hpp"
#include"restrain.hpp"
#include"functions.hpp"
#include"Eigen/Core"
#include"Eigen/Dense"
class beam{
private:
    std::vector<load> loads;
    std::vector<restrain> restrains;
    double E;
    double I;
    double Lmax;
    double Lmin;
    double length;
    int type;
    std::vector<funcInfo> __calcTheta(void);
    std::vector<funcInfo> __calcBend(void);
public:
    //约束类型
    enum res_type{
        M_1,
        F_2
    };
    beam()=default;
    ~beam()=default;
    beam(std::vector<load>&& loads,std::vector<restrain>&& restrains){
        this->loads = loads;
        this->restrains = restrains;
        this->E = 0;
        this->I = 0;
    }
    beam(std::vector<load>&& loads,std::vector<restrain>&& restrains,double E,double I){
        this->loads = loads;
        this->restrains = restrains;
        this->E = E;
        this->I = I;
    }
    ////////////////////计算////////////////////
    bool calcRestrain();
    std::vector<funcInfo> calcTheta(void);
    std::vector<funcInfo> calcBend(void);
    void calcL();
    ////////////////////设置和修改参数////////////////////
    void setE(double E){
        this->E = E;
    }
    void setI(double I){
        this->I = I;
    }
    void addLoad(load&& load){
        this->loads.push_back(load);
    }
    void addLoad(load& load){
        this->loads.push_back(load);
    }
    void addRestrain(restrain& restrain){
        this->restrains.push_back(restrain);
    }
    void addRestrain(restrain&& restrain){
        this->restrains.push_back(restrain);
    }
    void eraseLoad(int index){
        this->loads.erase(loads.begin()+index);
    }
    void eraseRestrain(int index){
        this->restrains.erase(restrains.begin()+index);
    }
    void setLoad(int index,load&& load){
        this->loads[index] = load;
    }
    void setRestrain(int index,restrain&& restrain){
        this->restrains[index] = restrain;
    }
    
    void listLoad(){
        int cnt=0;
        for(auto i:this->loads){
            
            const char* str[]={
                "M",
                "F",
                "q"
            };
            printf("LoadNum\t大小\t分布\t\t类型\n");
            printf("%d\t%.2f\t%.2f\t%.2f\t%s\n",\
            cnt,i.getConstantValue(),i.getSectionValue()[0],i.getSectionValue()[1],str[i.getRank()]);
            cnt++;
        }
    }
    void listRestrain(){
        int cnt=0;
        const char* str[]={
            "M",
            "F"
        };
        printf("ResNum\tF大小\tM大小\t位置\t类型\n");        
        for(auto i:this->restrains){
            
            printf("%d\t%.2f\t%.2f\t%.2f\t%s\n",\
            cnt,i.getFValue(),i.getMValue(),i.getPoint(),str[i.getRank()]);
            cnt++;
        }
    }
};



/*******计算约束*******/
bool beam::calcRestrain(){
    //读取数据
    int freedom = 3;
    int resMomentNum = 0;
    int resForceNum = 0;
    double resMomentloc=0;
    double resForceloc[2]={0,0};
    //记录索引
    std::vector<restrain>::iterator resMomentIter=restrains.begin();
    std::vector<restrain>::iterator resForceIter[2]={restrains.begin(),restrains.begin()};
        
    for(auto i=this->restrains.begin();i!=this->restrains.end();i++){
        if(i->getRank()==restrain::F){
            freedom--;
            resForceloc[resForceNum]=i->getPoint();
            resForceIter[resForceNum]=i;
            resForceNum++;
        }
        if(i->getRank()==restrain::M){
            freedom=freedom-2;
            resMomentloc=i->getPoint();
            resMomentIter=i;
            resMomentNum+=1;
        }
    }
    if(freedom!=1){
        printf("freedom error\n");
        return false;
    }
    //当自由度为0时，只能存在1个力矩&力约束，或者两个铰链（力）约束两种情况
    //计算y方向力
    double F = 0;
    for(auto i:this->loads){
        if(i.getRank()==load::F){
            F += i.getConstantValue();
        }
        if(i.getRank()==load::q){
            F += i.getConstantValue()*(i.getSectionValue()[1]-i.getSectionValue()[0]);
        }
    }

    //计算负载力矩
    double M = 0;
    for(auto i:loads){
        if(i.getRank()==load::F){
            M += i.getConstantValue()*(i.getSectionValue()[0]-resMomentloc);
        }
        if(i.getRank()==load::q){
            M += i.getConstantValue()\
                *(i.getSectionValue()[1]-i.getSectionValue()[0])\
                *((i.getSectionValue()[0]+i.getSectionValue()[1])/2-resMomentloc);
            }
        if(i.getRank()==load::M){
            M += i.getConstantValue();
        }
    }
    if(resMomentNum==0){
            this->type=F_2;
            Eigen::Matrix2f A(2,2);
            A(0,0) = 1;
            A(0,1) = 1;
            A(1,0) = resForceIter[0]->getPoint();
            A(1,1) = resForceIter[1]->getPoint();
            Eigen::Matrix<float,2,1>  b;
            b(0,0)=-F;
            b(1,0)=-M;
            Eigen::Matrix<float,2,1> x = A.inverse()*b;
            resForceIter[0]->setFValue(x(0,0));
            resForceIter[0]->setMValue(0);
            resForceIter[1]->setFValue(x(1,0));
            resForceIter[0]->setMValue(0);
    }
    if(resMomentNum==1){
            this->type=M_1;
            resMomentIter->setFValue(-F);
            resMomentIter->setMValue(F*resMomentIter->getPoint()-M);
    }
    return true;

}
//为计算常数项或部分常数项
std::vector<funcInfo> beam::__calcTheta(void){
    //std::vector<funcInfo>bend;
    std::vector<funcInfo>Ftheta;
    calcL();
    //计算所有theta函数

    for(auto l:this->loads){
        std::vector<funcInfo>temp;
        temp=calcThetaFuncInfo(l);
        for(auto i:temp){
            //printfunc(i);
            if(i.k>=0.00001||i.k<=-0.00001){
                Ftheta.push_back(i);
            }
        }
    }
    for(auto r:this->restrains){
        std::vector<funcInfo>temp;
        temp=calcThetaFuncInfo(r);
        for(auto i:temp){
            if(i.k>=0.00001||i.k<=-0.00001){
                Ftheta.push_back(i);
            }
        }
    }
    if(type==M_1){
        printf("\ntype:M_1\n");
        //计算theta代表的函数在矩约束处的theta值
        funcInfo edge_funcInfo;//边缘条件函数a=-inf,n=0,即退化为常数

        edge_funcInfo.a=-inf;
        edge_funcInfo.n=0;
        edge_funcInfo.k=0;//初始化
        double x=0;
        for(auto r:restrains){
            if(r.getRank()==restrain::M){
                x=r.getPoint();
            }
        }
        
        for(auto i:Ftheta){
            double temp;
            bool ret=calcfuncValue(x,temp,i.k,i.a,i.n);
            if(ret==true){
                edge_funcInfo.k-=temp;
            }
        }
        if(edge_funcInfo.k>=0.01&&edge_funcInfo.k<=-0.01){
            printfunc(edge_funcInfo,"\n");
            Ftheta.push_back(edge_funcInfo);
        }
    }
    
    for(auto it=Ftheta.begin();it<Ftheta.end();it++){
        printfunc(*it," ");
    }
    return Ftheta;
}
//计算部分常数项
std::vector<funcInfo> beam::__calcBend(void){
    std::vector<funcInfo>bend;
    std::vector<funcInfo>theta;
    theta=__calcTheta();
    for(auto i:theta){
        i.k=i.k/(i.n+1);
        i.n++;
        bend.push_back(i);
    }
    if(type==M_1){
        funcInfo edge_bendFuncs[1];
        double x;
        for(auto r:restrains){
            if(r.getRank()==restrain::M){
                edge_bendFuncs[0].a=r.getPoint();
                edge_bendFuncs[0].n=1;
                x=r.getPoint();
                break;
            }
        }
        edge_bendFuncs[0].k=0;
        for(auto i:bend){
            double y;
            calcfuncValue(x,y,i.k,i.a,i.n);
            edge_bendFuncs[0].k-=y;
        }
        if(edge_bendFuncs[0].k>0.0001||edge_bendFuncs[0].k<-0.0001){
            bend.push_back(edge_bendFuncs[0]);
        }
    }
    if(type==F_2){
        funcInfo edge_bendFuncs[2];
        double x[2];
        int cnt=0;
        for(auto r:restrains){
            if((r.getRank()==restrain::M)&&cnt==0){
                edge_bendFuncs[0].a=r.getPoint();
                edge_bendFuncs[0].n=0;
                x[0]=r.getPoint();
            }
            if((r.getRank()==restrain::M)&&cnt==1){
                edge_bendFuncs[1].a=r.getPoint();
                edge_bendFuncs[1].n=0;
                x[1]=r.getPoint();
            }
        }
        edge_bendFuncs[0].k=0;
        for(auto i:bend){
            double y[2];
            calcfuncValue(x[0],y[0],i.k,i.a,i.n);
            calcfuncValue(x[1],y[1],i.k,i.a,i.n);
            edge_bendFuncs[0].k-=y[0];
            edge_bendFuncs[1].k-=y[1];
        }
        for(int i=0;i<2;i++){
            if(edge_bendFuncs[i].k>0.0001||edge_bendFuncs[i].k<-0.0001){
                bend.push_back(edge_bendFuncs[i]);
            }
        }
    }
}
void beam::calcL(){
    double Lmax=0;
    double Lmin=0;
    double length=0;
    for(auto i:this->loads){
        if(Lmax>i.getSectionValue()[1]){
            Lmax = i.getSectionValue()[1];
        }
        if(Lmax>i.getSectionValue()[0]){
            Lmax = i.getSectionValue()[0];
        }
        if(Lmin<i.getSectionValue()[1]){
            Lmin = i.getSectionValue()[1];
        }
        if(Lmin<i.getSectionValue()[0]){
            Lmin = i.getSectionValue()[0];
        }
    }
    for(auto i:this->restrains){
        if(Lmax>i.getPoint()){
            Lmax = i.getPoint();
        }
        if(Lmin<i.getPoint()){
            Lmin = i.getPoint();
        }
    }
    this->Lmax=Lmax;
    this->Lmin=Lmin;
    length=Lmax-Lmin;
    this->length=length;
}