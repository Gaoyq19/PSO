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
using namespace std;
class ClusterAgent{
public:
    vector<shared_ptr<Particle>> particles;
    shared_ptr<Particle> gbestptr;
    int sequence;
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
        
        for (auto i : particles) {
            i->calculate(assist);
            int makespan = i->get_makespan();
            k += makespan;
            
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
};
#endif /* ClusterAgent_hpp */
