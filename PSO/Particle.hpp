//
//  Particle.hpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/23.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include <random>
#include "Assist.hpp"
using namespace std;
//辅助计算、构造，方便根据两个向量计算，记录每个操作在哪个机器执行及开始结束时间，以及makespan
class Particle{
public:
    int cluster;
    Particle(){}
    Particle(Assist &assist); //初始化向量，默认随机生成向量
    void calculate(Assist &assist); //计算makespan
    void update(int k, int iterations,const Particle &gbest);
    void updatebest(int k, int iterations,const Particle &gbest);
    void toJson(Assist &assist); //将向量所代表的意义转为json格式文件进行储存，便于可视化展示
    int get_makespan() const{
        return makespan;
    }
    int get_size() const{
        return size;
    }
    vector<int> get_jobVec() const{
        return jobVec;
    }
    vector<int> get_machineVec() const{
        return machineVec;
    }
    void setp(){
        if (successTimes > 15) {
            p *= 2;
        }else if (failureTimes > 5){
            p /= 2;
        }
    }
    void setzero(){
        successTimes = 0;
        failureTimes = 0;
    }
    //void update(Particle lbest);//根据SS对粒子位置进行更新
       //void makeSwapSequence();//根据聚类最佳、个人最佳求得粒子的SS

    //构造函数中赋值
    int size; //jobVec的长度
    vector<int> jobVec;
    vector<int> machineVec;
    vector<double> v_jobVec;
    vector<double> v_machineVec;
    vector<int> pbestJobVec; //personalbest向量
    vector<int> pbestMachineVec; //personalbest向量
    int makespan = INT_MAX;
    int pmakspan = INT_MAX;
    double w_Job = 0.6;
    double w_Machine = 0.6;
//    double vMin_Job;
//    double vMax_Job;
//    double vMin_Machine;
//    double vMax_Machine;
    double c1_Machine = 1.8;
    double c2_Machine = 1.6;
    double c1_Job = 1.8;
    double c2_Job = 1.6;
    double p = 1;
    int successTimes = 0;
    int failureTimes = 0;
    int e_s = 15;
    int e_f = 5;
//    double c1_Machine = 2.0;
//    double c2_Machine = 2;
//    double c1_Job = 2;
//    double c2_Job = 2;
    string fillZero(const string &s); //辅助json数据的构造
};
#endif /* Particle_hpp */
