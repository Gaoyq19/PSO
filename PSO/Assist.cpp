//
//  Assist.cpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/27.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#include "Assist.hpp"
#include "Machine.hpp"
#include "Job.hpp"
void Assist::reset(){
    for (int i = 1; i < jobs.size(); i++) {
        jobs[i].reset();
    }
    for (int i = 1; i <= machines.size(); i++) {
        machines[i].reset();
    }
}
