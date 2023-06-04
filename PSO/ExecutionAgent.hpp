//
//  ExecutionAgent.hpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/23.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#ifndef ExecutionAgent_hpp
#define ExecutionAgent_hpp

#include <stdio.h>
#include <iostream>
#include "Particle.hpp"
#include "ClusterAgent.hpp"
#include "Job.hpp"
#include <vector>
using namespace std;
class Assist;

class ExecutionAgent{
public:
    ExecutionAgent(); //读文件，对jobs，machines进行初始化
    void initia(Assist &assist){
        populations.reserve(500);
        //agents.reserve(4);
        for (int i = 0; i < 500; i++) {
            populations.emplace_back(make_shared<Particle>(assist));//共享指针。EA和CA管理同一份对象内存
            populations[i]->calculate(assist);
            //sort(populations.begin(),populations.end(),[](const Particle* &a, const Particle* &b) -> bool {return a->get_makespan() < b->get_makespan();});
        }
    }
    void make_cluster(Assist &assist){
        for (int i = 0; i < 500; i++) {
            agent.add_particle(populations[i]);
        }
        for (int i = 0; i < 1000; i++) {
            agent.calculate(assist);
            agent.update(i);
        }
//        clusters.push_back(populations[0]);
//        for (int i = 1; i < populations.size(); i++) {
//            ;
//        }
        
    }
    vector<Job> get_jobs() const{
        return jobs;
    }
     vector<Machine> get_machines() const{
        return machines;
    }
    //辅助构造Assit类
    int get_operationsNumber() const{
        return operations_n;
    }
friend class Assist;
private:
    ClusterAgent agent;
    vector<Job> jobs;
    vector<Machine> machines;
    vector<shared_ptr<Particle>> populations;
    vector<ClusterAgent> agents;
    int jobs_n;
    int machines_n;
    int operations_n;
    void splitStr(vector<int> &arr, string &s);//分割字符串，将每个数字存入vector arr
};
#endif /* ExecutionAgent_hpp */
