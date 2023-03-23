//
//  ExecutionAgent.cpp
//  PSO
//
//  Created by 高裕淇 on 2023/3/23.
//  Copyright © 2023 高裕淇. All rights reserved.
//

#include "ExecutionAgent.hpp"

#include <iostream>

#include "Operation.hpp"
#include <fstream>
using namespace std;
ExecutionAgent::ExecutionAgent(){
    ifstream file;
    file.open("/Users/gggyq/Downloads/TextData/Monaldo/Fjsp/Job_Data/Brandimarte_Data/Text/Mk001.fjs", ios_base::in);//文件第一行为job数量、machine数量、avg_per_op(未使用)

    if (!file.is_open())
    {
        cout << "打开文件失败";
    }
    string s;   //读取每行
    string str;     //每个数字
    vector<int> tmp(3);
    getline(file, s);   //读取第一行
    splitStr(tmp,s);
    jobs_n = tmp[0];
    machines_n = tmp[1];
    jobs.resize(jobs_n + 1);
    machines.resize(machines_n + 1);
    
    //读取每个job的所有operations
    for (int index = 1; index <= jobs_n; index++) {
        getline(file, s);
        vector<int> arr(1);
        splitStr(arr, s); //第一个数为operations的个数，其余以此形式存储（n,s1,t1,..,sn,tn），n代表每个操作可在n个机器执行，s为机器序号，t代表执行时间。
        int k = 1;
        int size = arr[0];
        operations_n += size;
        jobs[index].set_opr(size + 1); //避免vector动态分配
        for (int i = 0; i < size; i++) {
            //每个job添加所有的操作，以及机器与耗时
            Operation opr;
            int n = arr[k]; //arr[k]代表n，arr[k+1]代表s，arr[k+2]代表t
            for (int j = 0; j < n; j++) {
                
                opr.add_machine(arr[k + 1], arr[k + 2]);
                k += 2;
            }
            jobs[index].add_opr(opr, index); 
            k++;
        }
    }
    file.close();
}
void ExecutionAgent::splitStr(vector<int> &arr, string &s){
    string str;
    int k = 0;
    for (int i = 0; i < s.size(); i++) {
        if(!isdigit(s[i]) && !str.empty()){
            arr[k] = stoi(str);
            k++;
            str = "";
            if (arr.size() == 1) {
                int size = arr[0] * (machines_n * 2 + 1) + 1; //size的最大值，避免动态分配
                arr.resize(size);
            }
        }else if(isdigit(s[i])){
            str += s[i];
        }
    }
    if (str != "") {
        arr[k] = stoi(str);
    }
}
