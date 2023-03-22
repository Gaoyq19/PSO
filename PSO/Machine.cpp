//
//  Machine.cpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/22.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#include "Machine.hpp"
Machine::Machine():end(0),available(true){}
int Machine::finished_time(){
    return end;
}
bool Machine::isAvailable(){
    return available;
}
void Machine::execute(int end){
    this->end = end;
}
