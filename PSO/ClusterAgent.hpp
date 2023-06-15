//
//  ClusterAgent.hpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/27.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#ifndef ClusterAgent_hpp
#define ClusterAgent_hpp
#include <iostream>
#include <stdio.h>
#include "Particle.hpp"
#include "json/json.h"
#include <fstream>
using namespace std;
class ClusterAgent{
public:
    vector<double> arrAvg;
    vector<double> arrBest;
    vector<shared_ptr<Particle>> particles;
    shared_ptr<Particle> gbestptr;
    int cluster;
    int makespan_gbest = INT_MAX;
    Particle gbest;
    ClusterAgent(){}
    void add_particle(shared_ptr<Particle> p){
        particles.push_back(p);
        
    }
    void add_particle2(vector<shared_ptr<Particle>> &popu){
        for (int i = 0; i < 300; i++) {
            particles[i] = popu[i];
        }
    }
    void calculate(Assist &assist){
        double k = 0;
        int minMakspan = INT_MAX;
        shared_ptr<Particle> minParticle;
        for (auto i : particles) {
            i->calculate(assist);
            int makespan = i->get_makespan();
            k += makespan;
            if (minMakspan > makespan) {
                minMakspan = makespan;
                minParticle = i;
            }
            if (i == gbestptr) {
                if (makespan >= makespan_gbest) {
                    gbestptr->failureTimes++;
                    gbestptr->successTimes = 0;
                }else{
                    gbestptr->failureTimes = 0;
                    gbestptr->successTimes++;
                    makespan_gbest = makespan;
                    gbest = *i;
                    gbestptr = i;
                }
            }else{
                if (!gbestptr) {
                    gbestptr = i;
                }
                if (makespan_gbest > makespan) {
                    makespan_gbest = makespan;
                    gbestptr->setzero();
                    gbest = *i;
                    gbestptr = i;

                }
            }
           
            
        }
        if (gbestptr->get_makespan() > minMakspan) {
            gbestptr->setzero();
            gbestptr = minParticle;
        }
        arrAvg.push_back(double(k) / particles.size());
        arrBest.push_back(makespan_gbest);
        cout<<gbestptr->get_makespan()<<' '<< double(k) / particles.size() <<' '<< makespan_gbest << ' '<< particles[0]->cluster <<endl;
        
    }
    void update(int k, int iterations){
        for (auto i : particles) {
            if (i == gbestptr) {
                i->updatebest(k, iterations, gbest);
            }else{
                i->update(k, iterations, gbest);
            }
        }
    }
    void showpro(){
        string str1 = to_string(particles[0]->cluster) + "procAvg.json";
        string str2 = to_string(particles[0]->cluster) + "procBest.json";
        
        Json::Value rootAvg;
        for (int i = 0; i < arrAvg.size(); i++) {
            Json::Value x;
            x[to_string(i)] = arrAvg[i];
            rootAvg["x:y"].append(x);
        }
        Json::Value rootBest;
        for (int i = 0; i < arrBest.size(); i++) {
            Json::Value x;
            x[to_string(i)] = arrBest[i];
            rootBest["x:y"].append(x);
        }
        /*缩进输出到终端*/
        Json::StyledWriter sw;
        //        cout << sw.write(root) << endl << endl;
        /*输出到JSON文件*/
        ofstream procFile(str1, ios::trunc | ios::out );
        if (!procFile.is_open()){
            cout << "Fail to pen des.jons";
            return;
        }
        procFile << sw.write(rootAvg);
        procFile.close();
        /*输出到JSON文件*/
        ofstream procFileBest(str2, ios::trunc | ios::out );
        if (!procFileBest.is_open()){
            cout << "Fail to pen procBest.jons";
            return;
        }
        procFileBest << sw.write(rootBest);
        procFileBest.close();
    }
};
#endif /* ClusterAgent_hpp */
