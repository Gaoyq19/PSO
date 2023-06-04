//
//  main.cpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/22.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#include <iostream>
#include "json/json.h"
#include "ExecutionAgent.hpp"
#include "Particle.hpp"
#include <random>
using namespace std;

int main(int argc, const char * argv[]) {
    srand(time(NULL));
    ExecutionAgent eA;
    Assist assist(eA.get_jobs(),eA.get_machines(),eA.get_operationsNumber());
    eA.initia(assist);
    eA.make_cluster(assist);
//    Assist assist(eA);
//    Particle p(assist);
//    
//    p.calculate(assist);
//    p.toJson(assist);
    cout<<1;
    return 0;
}
