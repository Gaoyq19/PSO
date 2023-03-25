//
//  Operation.cpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/23.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#include "Operation.hpp"

int Operation::check(int machine_i){
    if (execution_time.count(machine_i)) {
        return machine_i;
    }else{
        int m = INT_MAX;
        int m_i = -1;
        for (auto i : execution_time) {
            if(i.second < m){
                m = i.second;
                m_i = i.first;
            }
        }
        return m_i;
    }
}
