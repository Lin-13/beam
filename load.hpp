#define LOAD_HPP
#include<vector>
#include<stdio.h>
class load
{
protected:
    /* data */
    int rank;
    std::vector<double> section;
    double point;
    double constant;
    int config;
public:
    enum rank_flags{
        M,
        F,
        q
    };
    load()=default;
    ////////////////////构造器////////////////////
    /*
    * @Breif: data安顺序为：载荷大小，分布
    * @Param data: vector of doubles
    * @Param rank: M 矩负载，只会使用第一个点,F 集中力负载，只会使用data的第一个点,q 均布负载，会使用所有点进行拟合
    */
    load(std::vector<double>&& data,int rank){
        this->rank = rank;
        this->constant=data[0];
        if(rank==M||rank==F){
            this->section.push_back(data[1]);
            this->point=data[1];
            this->config = 1;
        }else if(rank==q){
            this->section.push_back(data[1]);
            this->section.push_back(data[2]);
            this->point=(data[1]+data[2])/2;
            this->config = 1;
        }else{
            printf("wrong\n");
            this->config=0;
        }
        
    }
    /*
    * @Breif: 该函数使用vector描述
    * @Param data: vector of doubles
    * @Param rank: M 矩负载，只会使用第一个点,F 集中力负载，只会使用data的第一个点,q 均布负载，会使用所有点进行拟合
    */
    load(std::vector<double>& data,int rank){
        this->rank = rank;
        this->constant=data[0];
        if(rank==M||rank==F){
            this->section.push_back(data[1]);
            this->point=data[0];
            this->config = 1;
        }else if(rank==q){
            this->section.push_back(data[1]);
            this->section.push_back(data[2]);
            this->point=(data[1]+data[2])/2;
            this->config = 1;
        }else{
            printf("wrong rank\n");
            this->config=0;
        }
    }
    /*
    * @Brief: 负载为常数
    * @Param constant: 常数
    * @Param section: 分段,当rank为q时为区间，需要vector大小为2，当rank为M或者F时为点，vector大小为1
    */
    load(double constant,std::vector<double>&& section,int rank){
        this->rank=rank;
        this->constant = constant;
        this->config=1;
        if(rank==q){
            if(section.size()!=2){
                printf("wrong section size\n");
                this->config=0;
            }
            this->section.push_back(section[0]);
            this->section.push_back(section[1]);
            this->point=(section[0]+section[1])/2;
        }
        if(rank==M||rank==F){
            if(section.size()!=1){
                printf("wrong section size\n");
                this->config=0;
            }
            this->section=section;
            this->point=section[0];
        }
    }
    load(double constant,std::vector<double>& section,int rank){
        this->rank=rank;
        this->constant = constant;
        this->config=1;
        if(rank==q){
            if(section.size()!=2){
                printf("wrong section size\n");
                this->config=0;
            }
            this->section.push_back(section[0]);
            this->section.push_back(section[1]);
            this->point=(section[0]+section[1])/2;
        }
        if(rank==M||rank==F){
            if(section.size()!=1){
                printf("wrong section size\n");
                this->config=0;
            }
            this->section=section;
            this->point=section[0];
        }
    }
    /*
    * @brief: section不做检查
    * @Param constant: 负载常数
    * @Param section: 分段,当rank为q时为区间，需要数组大小为2，当rank为M或者F时为点，数组大小为1
    * @Param rank: M 矩负载，只会使用第一个点,F 集中力负载，只会使用data的第一个点,q 均布负载
    */
    load(double constant,double* section,int rank){
        this->rank=rank;
        this->constant = constant;
        this->config=1;
        if(rank==q){
            this->section=std::vector<double>(*section,*(section+1));
            this->point=(*section+*(section+1))/2;
        }
        if(rank==M||rank==F){
           this->section=std::vector<double>(*section);
           this->point=this->section[0];
        }
    }
    /////////////////////////////输出///////////////////////////////
    double getConstantValue(){
        return this->constant;
    }
    std::vector<double> getSectionValue(){
        if(this->rank!=q){
            printf("并非均布负载，可能存在错误\n");
        }
        return std::vector<double>(this->section);
   }
   std::vector<double> getData(){
       std::vector<double> data;
        data.push_back(this->constant);
        data.push_back(this->section[0]);
        if(this->rank==q){
            data.push_back(this->section[1]);
        }else{
            data.push_back(this->section[0]);
        }
        data.push_back(this->point);
        data.push_back(this->rank);
        data.push_back(this->config);
        return data;
   }
   int getRank(){
       return this->rank;
   }
   int getConfig(){
       return this->config;
   }
   double getPoint(){
       return this->point;
   }
};
