//
//  ClusterAgent.hpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/27.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#ifndef ClusterAgent_hpp
#define ClusterAgent_hpp

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
        for (auto i : particles) {
            i->calculate(assist);
        }
    }
    void update(){
        for (auto i : particles) {
            i->update();
        }
    }
};
#endif /* ClusterAgent_hpp */
