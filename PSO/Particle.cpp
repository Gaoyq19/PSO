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
#include <random>
using namespace std;
Particle::Particle(Assist &assist){
    int n = assist.jobs.size() - 1;
    size = assist.operations_n;
    jobVec.resize(size);
    machineVec.resize(size);
    v_jobVec.resize(size);
    v_machineVec.resize(size);
    for (int i = 0; i < assist.operations_n; i++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        double minVal = 0.0;
        double maxVal = 1.0;
        std::uniform_real_distribution<double> distribution(minVal, maxVal);
        double r1 =  distribution(gen);
        double r2 =  distribution(gen);
        int r = rand() % n + 1;
        while (assist.jobs[r].isFinished()) {   //随机选择一个未完成的作业
            r = rand() % n + 1;
        }
        jobVec[i] = r;
        machineVec[i] = assist.jobs[r].random_selectMachine();
        assist.jobs[r].execute();
        v_jobVec[i] = 0;
        v_machineVec[i] = 0;
    }
    assist.reset();
    pbestJobVec = jobVec;
    pbestMachineVec = machineVec;
    
}
void Particle::calculate(Assist &assist){
    int job_i;
    int machine_i;
    int start_time; //每次操作的开始时间（满足：当前任务的上个操作完工，机器空闲）
    int end;
    int fin = 0;
    for (int i = 0; i < jobVec.size(); i++) {
        if (jobVec[i] >= assist.jobs.size()) {
            jobVec[i] = assist.jobs.size() - 1;
        }
        if (jobVec[i] < 1) {
            jobVec[i] = 1;
        }
        while(assist.jobs[jobVec[i]].isFinished()){
//            if(v_jobVec[i] > 0){
//                jobVec[i]++;
//                if (jobVec[i] >= assist.jobs.size()) {
//                    jobVec[i] = 1;
//                }
//            }else{
//                jobVec[i]--;
//                if (jobVec[i] == 0) {
//                    jobVec[i] = assist.jobs.size() - 1;
//                }
//            }
//            jobVec[i]--;
//            if (jobVec[i] == 0) {
//                jobVec[i] = assist.jobs.size() - 1;
//            }
            jobVec[i]++;
            if (jobVec[i] >= assist.jobs.size()) {
                jobVec[i] = 1;
            }
        }
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
void Particle::updatebest(int k, int iterations, const Particle &gbest){
    int n = jobVec.size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(0, 1);
    double r = distribution(gen);
    setp();
    for (int i = 0; i < n; i++) {
        double vlow = 3.5, vup = 4.5;
        double vMax = vup - (vup - vlow) * double(k) / double(iterations);
        double vMin = -vup + (vup - vlow) * double(k) / double(iterations);
        v_jobVec[i] = w_Job * v_jobVec[i] + gbest.jobVec[i] - jobVec[i] + p * (1 - 2 * r);
        v_machineVec[i] = w_Machine * v_machineVec[i] + gbest.machineVec[i] - machineVec[i] + p * (1 - 2 * r);
        if (v_jobVec[i] > vMax) {
            v_jobVec[i] = vMax;
        }
        if (v_jobVec[i] < vMin) {
            v_jobVec[i] = vMin;
        }
        if (v_machineVec[i] > vMax) {
            v_machineVec[i] = vMax;
        }
        if (v_machineVec[i] < vMin) {
            v_machineVec[i] = vMin;
        }
        jobVec[i] = ceil(v_jobVec[i] + jobVec[i]);
        machineVec[i] = ceil(v_machineVec[i] + machineVec[i]);
    }
}
void Particle::update(int k, int iterations, const Particle &gbest){
    std::random_device rd;
    std::mt19937 gen(rd());
    double minVal = 0.0;
    double maxVal = 1.0;
    std::uniform_real_distribution<double> distribution(minVal, maxVal);
    double r1_Job = distribution(gen);
    double r2_Job = distribution(gen);
    double r1_Machine = distribution(gen);
    double r2_Machine = distribution(gen);
    double r_Job = distribution(gen);
    double r_Machine = distribution(gen);
    w_Job = 0.9 - (0.5) * double(k) / double(iterations);
    w_Machine = 0.9 - (0.5) * double(k) / double(iterations);
//    c1_Job = 0.5 + (2) * double(k) / double(iterations); //前期小
//    c2_Job = 2.5 - (2) * double(k) / double(iterations); //前期大
//    c1_Machine = 0.5 + (2) * double(k) / double(iterations);
//    c2_Machine = 2.5 - (2) * double(k) / double(iterations);
    double vlow = 3.5, vup = 4.5;
    double vMax = vup - (vup - vlow) * double(k) / double(iterations);
    double vMin = -vup + (vup - vlow) * double(k) / double(iterations);
    

    int n = jobVec.size();
    for (int i = 0; i < n; i++) {
        v_jobVec[i] = w_Job * v_jobVec[i] + c1_Job * r1_Job * (gbest.jobVec[i] - jobVec[i]) + c2_Job * r2_Job * (pbestJobVec[i] - jobVec[i]);
        
        if (v_jobVec[i] > vMax) {
            v_jobVec[i] = vMax;
        }
        if (v_jobVec[i] < vMin) {
            v_jobVec[i] = vMin;
        }
        jobVec[i] = ceil(v_jobVec[i] + jobVec[i]);
        v_machineVec[i] = w_Machine * v_machineVec[i] + c1_Machine * r1_Machine * (gbest.machineVec[i] - machineVec[i]) + c2_Machine * r2_Machine * (pbestMachineVec[i] - machineVec[i]);
        
        if (v_machineVec[i] > vMax) {
            v_machineVec[i] = vMax;
        }
        if (v_machineVec[i] < vMin) {
            v_machineVec[i] = vMin;
        }
        machineVec[i] = ceil(v_machineVec[i] + machineVec[i]);
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
    
