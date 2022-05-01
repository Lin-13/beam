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
public:
    beam()=default;
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
    /*******计算约束*******/
    bool calcRestrain(){
        //读取数据
        int freedom = 3;
        int resMomentNum = 0;
        int resForceNum = 0;
        double resMomentloc=0;
        double resForceloc[2]={0,0};
        //记录索引
        std::vector<restrain>::iterator resMomentIter=restrains.begin();
        std::vector<restrain>::iterator resForceIter[2]={restrains.end(),restrains.end()};
        
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
            Eigen::Matrix2f A(2,2);
            A(0,0) = 1;
            A(0,1) = 1;
            A(1,0) = resForceIter[0]->getPoint();
            A(1,1) = resForceIter[1]->getPoint();
            Eigen::Matrix2f b(2,1);
            b(0,0)=-F;
            b(1,0)=-M;
            Eigen::Matrix2f x = A.inverse()*b;
            resForceIter[0]->setFValue(x(0,0));
            resForceIter[1]->setFValue(x(1,0));
        }
        if(resMomentNum==1){
            resMomentIter->setFValue(-F);
            resMomentIter->setMValue(F*resMomentIter->getPoint()-M);
        }
        return true;

    }
    std::vector<std::vector<double>>calcBend(double accuracy){
        std::vector<std::vector<double>>result;
    }
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
    void calcL(){
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
        for(auto i:this->restrains){
            printf("ResNum\tF大小\tM大小\t位置\t类型\n");
            printf("%d\t%.2f\t%.2f\t%.2f\t%s\n",\
            cnt,i.getFValue(),i.getMValue(),i.getPoint(),str[i.getRank()]);
            cnt++;
        }
    }
};