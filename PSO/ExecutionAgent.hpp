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
#include "Machine.hpp"
#include "Job.hpp"
#include <vector>
using namespace std;
class ExecutionAgent{
public:
    vector<Job> jobs;
    vector<Machine> machines;
    int jobs_n;
    int machines_n;
    ExecutionAgent(); //读文件，对jobs，machines进行初始化
    void splitStr(vector<int> &arr, string &s);//分割字符串，将每个数字存入vector arr
};
#endif /* ExecutionAgent_hpp */
