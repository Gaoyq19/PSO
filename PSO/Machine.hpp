//
//  Machine.hpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/22.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#ifndef Machine_hpp
#define Machine_hpp

#include <stdio.h>
class Machine{
private:
    bool available;
    int end;
public:
    Machine():end(0),available(true){}
    int finished_time(){
        return end;
    }; //返回机器完成当前最后一个操作的时间
    bool isAvailable(){
        return available;
    }
    void execute(int end){
        this->end = end;
    } //给机器添加一个操作，并更新完成时间
    
};
#endif /* Machine_hpp */
