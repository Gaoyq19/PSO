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
#include <vector>
#include <string>
#include "Job.hpp"
#include "ExecutionAgent.hpp"
#include "Machine.hpp"
using namespace std;
//辅助计算、构造，方便根据两个向量计算，记录每个操作在哪个机器执行及开始结束时间，以及makespan
class Assist{
public:
    const int operations_n;
    vector<Job> jobs; 
    vector<Machine> machines;
    Assist(const ExecutionAgent &eA):jobs(eA.get_jobs()),machines(eA.get_machines()),operations_n(eA.get_operationsNumber()){}
    void reset();
};
class Particle{
public:
    void calculate(Assist &assist); //计算makespan
    void toJson(Assist &assist); //将向量所代表的意义转为json格式文件进行储存，便于可视化展示
    Particle(Assist &assist); //初始化向量，默认随机生成向量
private:
    vector<int> pbestJobVec; //personalbest向量
    vector<int> pbestMachineVec; //personalbest向量
    vector<int> jobVec;
    vector<int> machineVec;
    int makespan;
    int pmakspan;
    string fillZero(const string &s); //辅助json数据的构造
};
#endif /* Particle_hpp */
