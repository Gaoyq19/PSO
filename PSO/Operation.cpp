//
//  Operation.cpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/23.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#include "Operation.hpp"

int Operation::check(int machine_i){
    int diff = INT_MAX;
    int result = -1;
    for (int i = 0; i < machines.size(); i++) {
        int n = abs(machine_i - machines[i]);
        if (diff > n) {
            diff = n;
            result = machines[i];
        }
    }
    return  result;

}
