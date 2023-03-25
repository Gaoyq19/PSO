//
//  Particle.cpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/23.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#include "Particle.hpp"
#include "json/json.h"
#include <fstream>
#include <list>
#include <iostream>
using namespace std;
void Assist::reset(){
    for (int i = 1; i < jobs.size(); i++) {
        jobs[i].reset();
    }
    for (int i = 1; i <= machines.size(); i++) {
        machines[i].reset();
    }
}
Particle::Particle(Assist &assist){
    int n = assist.jobs.size() - 1;
    jobVec.resize(assist.operations_n);
    machineVec.resize(assist.operations_n);
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
}
void Particle::calculate(Assist &assist){
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
    makespan = fin;
    if (pmakspan > makespan) {
        pmakspan = makespan;
        pbestJobVec = jobVec;
        pbestMachineVec = machineVec;
    }
    assist.reset();
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
    
