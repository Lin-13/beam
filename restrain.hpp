#define RESTRAIN_HPP
#include<stdio.h>
class restrain{
private:
    double point;
    double Fvalue;
    double Mvalue;
    int rank;
public:
    double setMValue(double Mvalue);
    double setFValue(double Fvalue);
    int isknown;
    enum restrain_flags{
        M,
        F
    };
    restrain()=default;
    restrain(double point,int rank){
        this->point=point;
        this->isknown=0;
        this->Fvalue=0;
        this->Mvalue=0;
        if(rank==M||rank==F){
            this->rank=rank;
        }
        else{
            printf("rank error\n");
        }
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
double restrain::setFValue(double Fvalue){
    this->Fvalue=Fvalue;
    this->isknown++;
    return this->Fvalue;
}
double restrain::setMValue(double Mvalue){
    this->Mvalue=Mvalue;
    this->isknown++;
    return this->Mvalue;
}