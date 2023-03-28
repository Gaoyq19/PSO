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
class Job;
class Machine;
class Assist{
public:
    const int operations_n;
    vector<Job> jobs;
    vector<Machine> machines;
    Assist(vector<Job> j, vector<Machine> m, int n):jobs(j),machines(m),operations_n(n){}
    void reset();
};

#endif /* Assist_hpp */
