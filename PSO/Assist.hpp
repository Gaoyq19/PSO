//
//  Assist.hpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/27.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#ifndef Assist_hpp
#define Assist_hpp

#include <stdio.h>
#include "ExecutionAgent.hpp"
class Assist{
public:
    const int operations_n;
    vector<Job> jobs;
    vector<Machine> machines;
    Assist(const ExecutionAgent &eA):jobs(eA.get_jobs()),machines(eA.get_machines()),operations_n(eA.get_operationsNumber()){}
    void reset();
};

#endif /* Assist_hpp */
