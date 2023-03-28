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
    void calculate(Assist &assist); //计算makespan
    void toJson(Assist &assist); //将向量所代表的意义转为json格式文件进行储存，便于可视化展示
    Particle(Assist &assist); //初始化向量，默认随机生成向量
    void update();//根据SS对粒子位置进行更新
    void makeSwapSequence();//根据聚类最佳、个人最佳求得粒子的SS
    int get_makespan() const{
        return makespan;
    }
    int static get_dis(shared_ptr<Particle> p1, shared_ptr<Particle> p2);
private:
    //构造函数中赋值
    vector<int> jobVec;
    vector<int> machineVec;
    vector<int> pbestJobVec; //personalbest向量
    vector<int> pbestMachineVec; //personalbest向量
    vector<int> lbestJobVec; //localbest向量
    vector<int> lbestMachineVec; //localbest向量
    //makeSS()进行赋值
    vector<vector<int>> swapSequence_pJ; //个人最佳作业向量
    vector<vector<int>> swapSequence_pM; //个人最佳机器向量
    vector<vector<int>> swapSequence_lbJ; //聚类最佳作业向量
    vector<vector<int>> swapSequence_lbM; //聚类最佳机器向量
    int makespan = -1;
    int pmakspan = INT_MAX;
    double c1 = 0.6;
    double c2 = 0.8;;
    string fillZero(const string &s); //辅助json数据的构造
};
#endif /* Particle_hpp */
