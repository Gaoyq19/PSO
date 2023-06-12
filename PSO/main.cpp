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

    ExecutionAgent eA;
    Assist assist(eA.get_jobs(),eA.get_machines(),eA.get_operationsNumber());
    eA.initia(assist,8);
    //eA.oneCluster(assist);
    eA.make_cluster(assist);
    std::vector<std::thread> threads;
    for (int cluster = 0; cluster < 8; ++cluster) {
        threads.emplace_back(&ExecutionAgent::PSO, &eA,assist, cluster);
        //eA.PSO(assist, cluster);
    }
    // 等待所有线程完成
    for (std::thread& thread : threads) {
        thread.join();
    }
//    for (int i = 9; i < 100; i++) {
//        eA.contin(assist);
//    }

    //    Assist assist(eA);
//    Particle p(assist);
//
//    p.calculate(assist);
//    p.toJson(assist);
    cout<<1;
    return 0;
}
