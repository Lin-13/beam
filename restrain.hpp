#define RESTRAIN_HPP
#include<stdio.h>
class restrain{
private:
    double point;
    double Fvalue;
    double Mvalue;
    int isknown;
    int rank;
public:
    enum restrain_flags{
        M,
        F
    };
    restrain()=default;
    restrain(double point,int rank){
        this->point=point;
        this->isknown=false;
        this->Fvalue=0;
        this->Mvalue=0;
        if(rank==M||rank==F){
            this->rank=rank;
        }
        else{
            printf("rank error\n");
        }
    }
    double setFValue(double Fvalue){
        this->Fvalue=Fvalue;
        this->isknown=true;
        return this->Fvalue;
    }
    double setMValue(double Mvalue){
        this->Mvalue=Mvalue;
        this->isknown=true;
        return this->Mvalue;
    }
    double getPoint(){
        return this->point;
    }
    double getFValue(){
        return this->Fvalue;
    }
    double getMValue(){
        return this->Mvalue;
    }
    int getRank(){
        return this->rank;
    }
};