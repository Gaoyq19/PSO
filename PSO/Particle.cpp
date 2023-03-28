//
//  Particle.cpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/23.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#include "Particle.hpp"
#include "json/json.h"
#include "Job.hpp"
#include "Machine.hpp"
#include <fstream>
#include <list>
#include <iostream>
using namespace std;
Particle::Particle(Assist &assist){
    int n = assist.jobs.size() - 1;
    jobVec.reserve(assist.operations_n);
    machineVec.reserve(assist.operations_n);
    for (int i = 0; i < assist.operations_n; i++) {
        int r = rand() % n + 1;
        while (assist.jobs[r].isFinished()) {   //随机选择一个未完成的作业
            r = rand() % n + 1;
        }
        jobVec[i] = r;
        machineVec[i] = assist.jobs[r].random_selectMachine();
        assist.jobs[r].execute();
    }
    assist.reset();
    pbestJobVec = jobVec;
    pbestMachineVec = machineVec;
    
}
int Particle::get_dis(shared_ptr<Particle> p1, shared_ptr<Particle> p2){
    vector<int> tmp = p1->jobVec;
    vector<int> tmp1 = p1->machineVec;
    int dis = 0;
    for (int i = 0; i < p1->jobVec.size(); i++) {
        if (p2->jobVec[i] == tmp[i]) {
            continue;
        }
        for (int j = i + 1; j < p1->jobVec.size(); j++) {
            if (p2->jobVec[i] == tmp[j]) {
                dis++;
                swap(tmp[i],tmp[j]);
                break;
            }
        }
    }
    
    for (int i = 0; i < p1->machineVec.size(); i++) {
        if (p2->machineVec[i] == tmp1[i]) {
            continue;
        }
        for (int j = i + 1; j < p1->machineVec.size(); j++) {
            if (p2->machineVec[i] == tmp1[j]) {
                dis++;
                swap(tmp1[i],tmp1[j]);
                break;
            }
        }
    }
    return dis;
}
void Particle::calculate(Assist &assist){
    int job_i;
    int machine_i;
    int start_time; //每次操作的开始时间（满足：当前任务的上个操作完工，机器空闲）
    int end;
    int fin = 0;
    for (int i = 0; i < jobVec.size(); i++) {
        job_i = jobVec[i];
        //交换操作后，存在机器不执行该操作的可能，进行更改
        machineVec[i] = assist.jobs[job_i].check(machineVec[i]);
        machine_i = machineVec[i];
        start_time = max(assist.machines[machine_i].finished_time(), assist.jobs[job_i].finished_time());
        end = assist.jobs[job_i].execute(start_time, machine_i);
        assist.machines[machine_i].execute(end);
        fin = max(end, fin);
    }
    makespan = fin;
    if (pmakspan > makespan) {
        pmakspan = makespan;
        pbestJobVec = jobVec;
        pbestMachineVec = machineVec;
    }
    assist.reset();
}
void Particle::makeSwapSequence(){
    vector<int> tmp = jobVec;
    vector<int> tmp1 = machineVec;
    for (int i = 0; i < pbestJobVec.size(); i++) {
        if (pbestJobVec[i] == tmp[i]) {
            continue;
        }
        for (int j = i + 1; j < jobVec.size(); j++) {
            if (pbestJobVec[i] == tmp[j]) {
                swapSequence_pJ.push_back(vector<int>{i,j});
                swap(tmp[i],tmp[j]);
                break;
            }
        }
    }
    for (int i = 0; i < pbestMachineVec.size(); i++) {
        if (pbestMachineVec[i] == tmp1[i]) {
            continue;
        }
        for (int j = i + 1; j < machineVec.size(); j++) {
            if (pbestMachineVec[i] == tmp1[j]) {
                swapSequence_pM.push_back(vector<int>{i,j});
                swap(tmp[i],tmp[j]);
                break;
            }
        }
    }
    for (int i = 0; i < lbestJobVec.size(); i++) {
        if (lbestJobVec[i] == tmp[i]) {
            continue;
        }
        for (int j = i + 1; j < jobVec.size(); j++) {
            if (lbestJobVec[i] == tmp[j]) {
                swapSequence_lbJ.push_back(vector<int>{i,j});
                swap(tmp[i],tmp[j]);
                break;
            }
        }
    }
    for (int i = 0; i < lbestMachineVec.size(); i++) {
        if (lbestMachineVec[i] == tmp1[i]) {
            continue;
        }
        for (int j = i + 1; j < machineVec.size(); j++) {
            if (lbestMachineVec[i] == tmp1[j]) {
                swapSequence_lbM.push_back(vector<int>{i,j});
                swap(tmp[i],tmp[j]);
                break;
            }
        }
    }
}
void Particle::update(){
    default_random_engine e(time(0));
    makeSwapSequence();
    for (int i = 0; i < swapSequence_pJ.size(); i++) {
        uniform_real_distribution<double> r(0,1);
        if (r(e) < c1) {
            int k1 = swapSequence_pJ[i][0];
            int k2 = swapSequence_pJ[i][1];
            swap(jobVec[k1], jobVec[k2]);
        }
    }
    for (int i = 0; i < swapSequence_pM.size(); i++) {
        uniform_real_distribution<double> r(0,1);
        if (r(e) < c1) {
            int k1 = swapSequence_pM[i][0];
            int k2 = swapSequence_pM[i][1];
            swap(machineVec[k1], machineVec[k2]);
        }
    }
    for (int i = 0; i < swapSequence_lbJ.size(); i++) {
        uniform_real_distribution<double> r(0,1);
        if (r(e) < c2) {
            int k1 = swapSequence_lbJ[i][0];
            int k2 = swapSequence_lbJ[i][1];
            swap(jobVec[k1], jobVec[k2]);
        }
    }
    for (int i = 0; i < swapSequence_lbM.size(); i++) {
        uniform_real_distribution<double> r(0,1);
        if (r(e) < c2) {
            int k1 = swapSequence_lbM[i][0];
            int k2 = swapSequence_lbM[i][1];
            swap(machineVec[k1], machineVec[k2]);
        }
    }
}
string Particle::fillZero(const string &s){
    string str;
    for (int i = 0; i < 4 - s.size(); i++) {
        str += '0';
    }
    str += s;
    return  str;
}
void Particle::toJson(Assist &assist){
    //根据机器、工作向量进行计算，记录每个操作在机器i上执行及时间
    int job_i;
    int machine_i;
    int start_time;
    int end;
    int fin = 0;
    for (int i = 0; i < jobVec.size(); i++) {
        job_i = jobVec[i];
        machine_i = machineVec[i];
        start_time = max(assist.machines[machine_i].finished_time(), assist.jobs[job_i].finished_time());
        end = assist.jobs[job_i].execute(start_time, machine_i);
        assist.machines[machine_i].execute(end);
        fin = max(end, fin);
    }
    
    //将操作根据执行机器序号排序，有助于画图时美观
    vector<vector<Operation>> m(assist.machines.size());
    for (int i = 1; i < assist.jobs.size(); i++) {
        for (int j = 0; j < assist.jobs[i].size(); j++) {
            int k = assist.jobs[i].get_opr(j).get_machineNumber();
            m[k].push_back(assist.jobs[i].get_opr(j));
        }
    }
    
    Json::Value root;/*JSON文件的根节点*/
    for (int i = 1; i < assist.machines.size(); i++) {
        for (int j = 0; j < m[i].size(); j++) {
            Json::Value opr;
            const Operation &tmp = m[i][j];
            string str = "Job" + to_string(tmp.get_jobNumber()) + "-" + to_string(tmp.get_oprNumber());
            opr["Task"] = Json::Value("Machine:" + to_string(tmp.get_machineNumber()));
            opr["Start"] = Json::Value(fillZero(to_string(tmp.get_startTime())));
            opr["Finish"] = Json::Value(fillZero(to_string(tmp.get_endTime())));
            opr["Status"] = Json::Value("Job:" + to_string(tmp.get_jobNumber()));
            opr["Description"] = Json::Value(str);
            root["makespan"] = Json::Value(to_string(fin));
            root["operations"].append(opr);
            
        }
    }
    assist.reset();
    /*缩进输出到终端*/
    Json::StyledWriter sw;
    cout << sw.write(root) << endl << endl;
    /*输出到JSON文件*/
    ofstream desFile("des.json", ios::trunc | ios::out );
    if (!desFile.is_open()){
        cout << "Fail to pen des.jons";
        return;
    }
    desFile << sw.write(root);
    desFile.close();
}
    
