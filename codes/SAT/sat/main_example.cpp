#include <iostream>
#include <ctime>
#include <string>

#include "walksat.h"
#include "check.h"
#include "vitrualEnvCaucute.h"
#include "creatFlow.h"

using namespace std;

void start_cmd(int argc, char *argv[]){
    string ctrlstr=argv[1];
    if(argc<=1);
    /* walksat 入口，参数[算例.cnf 输出解文件.sol 输出变元权重文件.vw]
        "args":[
            "--walksat",
            "../specification_data/1ee0e949-d186-4b94-a0fd-efc55d06a7e2/0e30ff9a-bf2d-4a31-ad32-823faf6af0f7.cnf",
            "../solve/0e30ff9a-bf2d-4a31-ad32-823faf6af0f7.sol",
            "../var_weight/0e30ff9a-bf2d-4a31-ad32-823faf6af0f7.vw"
        ],
    */
    else if(ctrlstr=="--walksat"){
        if(argc==5){
            cout<<"begin"<<endl;
            WalkSat(argv[2],argv[3],argv[4],10,100);
            cout<<"end"<<endl;
        }
    }
    /* check 入口，参数[算例.cnf 解.sol]，返回sat/unsat提示
        "args":[
            "--walksat",
            "../specification_data/1ee0e949-d186-4b94-a0fd-efc55d06a7e2/0e30ff9a-bf2d-4a31-ad32-823faf6af0f7.cnf",
            "../solve/0e30ff9a-bf2d-4a31-ad32-823faf6af0f7.sol"
        ],
    */
    else if(ctrlstr=="--check"){
        if(argc==4){
            CheckSolve(argv[2],argv[3]);
        }
    }
    else if(ctrlstr=="--vitrual"){
        if(argc==5){
            vitrual(argv[2],argv[3],argv[4]);
        }
    }
    else if(ctrlstr=="--creatFlow"){
        if(argc==4){
            creatFlow(argv[2],argv[3]);
        }
    }
}

int main(int argc, char *argv[]){
    clock_t start=clock();
    start_cmd(argc,argv);
    clock_t end=clock();
    cout<<"time : "<<((double)end-start)/CLOCKS_PER_SEC<< "s\n";
    return 0;
}