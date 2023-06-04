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
private:
    vector<shared_ptr<Particle>> particles;
    shared_ptr<Particle> gbestptr;
    Particle gbest;
    int makespan_gbest = INT_MAX;
public:
    ClusterAgent(){}
    void add_particle(shared_ptr<Particle> p){
        particles.push_back(p);
    }
    void calculate(Assist &assist){
        double k = 0;
        for (auto i : particles) {
            i->calculate(assist);
            int makespan = i->get_makespan();
            k += makespan;
            if (makespan_gbest > makespan) {
                gbestptr = i;
                makespan_gbest = makespan;
            }
        }
        cout<<makespan_gbest<<' '<< k / 500 <<endl;
        
    }
    void update(int k){
        gbest = *gbestptr;
        for (auto i : particles) {
            i->update(k, *gbestptr);
        }
    }
};
#endif /* ClusterAgent_hpp */
