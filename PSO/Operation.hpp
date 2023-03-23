//
//  Operation.hpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/23.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#ifndef Operation_hpp
#define Operation_hpp

#include <stdio.h>
#include <map>
#include <vector>
using namespace std;
class Operation{
private:
    map<int, int> execution_time; //execution_time[key] : value, key号机器执行它花费的时间
    vector<int> machines; //可执行该操作的机器集合
    int job_number; //该操作所属job序号
    int opr_number; //操作序号
    int execution_machine; //执行该操作的机器
    int start = 0,end = 0;
public:
    void add_machine(int number, int time){  //记录可执行该操作的机器以及时间
        execution_time[number] = time;
        machines.push_back(number);
    }
    int execute(int start, int machine_i){ //记录在机器i上执行的开始、结束时间
        this->start = start;
        this->end = start + execution_time[machine_i];
        execution_machine = machine_i;
        return end;
    }
    int get_jobNumber() const{
        return job_number;
    }
    int get_oprNumber() const{
        return opr_number;
    }
    int get_machineNumber() const{
        return execution_machine;
    }
    int get_startTime() const{
        return start;
    }
    int get_endTime() const{
        return end;
    }
    void reset(){ //重置开始结束时间和执行机器
        start = 0;
        end = 0;
        execution_machine = 0;
    }
//    void setJob(int i);
//    void setOprNum(int i);
};
#endif /* Operation_hpp */
