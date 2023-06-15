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
#include <mutex>
#include <thread>
using namespace std;

class Assist;
class Point{
public:
    vector<double> jobVec;
    vector<double> machineVec;
    int cluster;
    Point(Particle &p,int k){
        cout<<"Point";
        vector<int> j = p.get_jobVec();
        vector<int> m = p.get_machineVec();
        for (int i = 0; i < p.get_size(); i++) {
            jobVec.push_back((double)j[i]);
            machineVec.push_back((double)m[i]);
        }
        //jobVec.insert(jobVec.begin(),p.get_jobVec().begin(), p.get_jobVec().end());
        //machineVec.insert(machineVec.begin(),p.get_machineVec().begin(), p.get_machineVec().end());
        cluster = k;
    }
};
class ExecutionAgent{
public:
    ExecutionAgent(); //读文件，对jobs，machines进行初始化
    void initia(Assist &assist, int k){
        srand(time(NULL));
        populations.reserve(size);
        //agents.reserve(4);
        for (int i = 0; i < size; i++) {
            populations.emplace_back(make_shared<Particle>(assist));//共享指针。EA和CA管理同一份对象内存
            //populations[i]->calculate(assist);
        }
        //随机选取一个particle作为中心点
        for (int i = 0 ; i < k; i++) {
            int r = rand() % size;
            Point* centroid = new Point(*populations[r], i);
            centroids.push_back(centroid);
        }
        int iteration = 0;
        int maxIterations = 1;
        while (iteration < maxIterations) {
            assignPointsToClusters(centroids);
            std::vector<Point*> newCentroids = calculateCentroids(k);
            cout<<iteration<<endl;
//            if (newCentroids == centroids) {
//                break; // 聚类中心不再变化，停止迭代
//            }
            //centroids = std::move(newCentroids);
            ++iteration;
        }
    }
    void oneCluster(Assist &assist){
        srand(time(NULL));
        agent.resize(1);
        for (int i = 0; i < size; i++) {
            agent[0].add_particle(populations[i]);
        }
        for (int i = 0; i < iterations; i++) {
            agent[0].calculate(assist);
            agent[0].update(i, iterations);
        }
        agent[0].showpro();
    }
    void make_cluster(Assist &assist){
        srand(time(NULL));
        agent.resize(8);
        for (int i = 0; i < size; i++) {
            int kind = populations[i]->cluster;
            agent[kind].add_particle(populations[i]);
        }
        
//        for (auto item : agent) {
//            int k = 0;
//            for (int i = 0; i < iterations; i++) {
//                k++;
//                item.calculate(assist);
//                item.update(i, iterations);
//                if(k == 40){
//                    if (item.gbest.get_makespan() < best.get_makespan()) {
//                        set_best(item.gbest);
//                    }
//                    if (item.gbest.get_makespan() > best.get_makespan()) {
//                        item.gbest = get_best();
//                        item.makespan_gbest = best.get_makespan();
//                    }
//                    k = 0;
//                }
//
//            }
//        }
        
    }
    void PSO(Assist assist, int index){
        int k = 0;
        for (int i = 0; i < iterations; i++) {
            k++;
            agent[index].calculate(assist);
            agent[index].update(i, iterations);
            if(k == 40){
                if (agent[index].gbest.get_makespan() < best.get_makespan()) {
                    set_best(agent[index].gbest);
                }
                if (agent[index].gbest.get_makespan() > best.get_makespan()) {
                    agent[index].gbest = get_best();
                    //agent[index].gbestptr = make_shared<Particle>(get_best());
                    agent[index].makespan_gbest = best.get_makespan();
                }
                k = 0;
            }
        }

    }
    void showpro(){
        for (auto i : agent) {
            i.showpro();
        }
    }
    void contin(Assist &assist){
        populations.clear();
        for (int i = 0; i < agent.size(); i++) {
            for (int j = 0; j < agent[i].particles.size(); j++) {
                agent[i].particles[j] = make_shared<Particle>(assist);
            }
        }
        for (auto item : agent) {
            int k = 0;
            for (int i = 0; i < iterations; i++) {
                k++;
                item.calculate(assist);
                item.update(i, iterations);
//                if(k == 40){
//                    if (item.gbest.get_makespan() < best.get_makespan()) {
//                        set_best(item.gbest);
//                    }
//                    if (item.gbest.get_makespan() > best.get_makespan()) {
//                        item.gbest = get_best();
//                        item.makespan_gbest = best.get_makespan();
//                    }
//                    k = 0;
//                }
                
            }
        }
    }
    
    Particle get_best(){
        lock_guard<std::mutex> lock(mtx);
        return best;
    }
    void set_best(Particle &p){
        lock_guard<std::mutex> lock(mtx);
        best = p;
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
private:
    mutex mtx;
    vector<ClusterAgent> agent;
    vector<Job> jobs;
    vector<Point*> centroids;
    vector<Machine> machines;
    vector<shared_ptr<Particle>> populations;
    int size = 500;
    int iterations = 5000;
    int jobs_n;
    int machines_n;
    int operations_n;
    void splitStr(vector<int> &arr, string &s);//分割字符串，将每个数字存入vector arr
    double calculateDistance(shared_ptr<Particle> p1,const Point &p2);
    vector<Point*> calculateCentroids(int k);
    void assignPointsToClusters(const std::vector<Point*>& centroids);
    Particle best;
};
#endif /* ExecutionAgent_hpp */
