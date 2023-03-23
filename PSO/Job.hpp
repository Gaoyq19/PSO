//
//  Job.hpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/23.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#ifndef Job_hpp
#define Job_hpp

#include <stdio.h>
#include "Operation.hpp"
#include <vector>
using namespace std;
class Job{
public:
    int finished_time() const{ //返回job当前已完成的操作的结束时间
        return end;
    }
    int size() const{
        return n;
    }
    Operation get_opr(int i) const{
        return operations[i];
    }
    bool isFinished() const{
        if (finished_n == n) {
            return true;
        } else {
            return false;
        }
    }
    int execute(int start, int machine_i){ // 开始时间、机器序号
        end = operations[finished_n].execute(start, machine_i);
        finished_n++;
        return end;
    }
    void execute(){ //空执行，辅助初始化
        finished_n++;
    }
    void set_opr(int size){
        operations.resize(size);
    }
    void add_opr(Operation opr, int i){
        opr.set_job(i);
        opr.set_oprNum(n);
        operations[n] = opr;
        n++;
    }
    void reset(){
        finished_n = 0;
        end = 0;
        for (int i = 1; i < operations.size(); i++) {
            operations[i].reset();
        }
    }
    int random_selectMachine(){
        return operations[finished_n].random_selectMachine();
    }
private:
    vector<Operation> operations;
    int n = 0; //操作的数量
    int finished_n = 0;
    int end = 0;
};
#endif /* Job_hpp */
