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
    shared_ptr<Particle> particle_LB;
    int makespan_LB;
public:
    void add_particle(shared_ptr<Particle> p){
        particles.push_back(p);
        int makespan = p->get_makespan();
        if (makespan_LB > makespan) {
            particle_LB = p;
            makespan_LB = makespan;
        }
    }
    void calculate(Assist &assist){
        double k = 0;
        for (auto i : particles) {
            i->calculate(assist);
            int makespan = i->get_makespan();
            k += makespan;
            if (makespan_LB > makespan) {
                particle_LB = i;
                makespan_LB = makespan;
            }
        }
        cout<<makespan_LB<<' '<< k / 100 <<endl;
    }
    void update(int k){
        for (auto i : particles) {
            i->update(k, *particle_LB);
        }
    }
};
#endif /* ClusterAgent_hpp */
