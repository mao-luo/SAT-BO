#include "genSolutions.h"

#include <stdlib.h>

#include <cmath>
#include <ctime>
#include <vector>

#include "dpll.h"
#include "ioBase.h"

using namespace std;

void InitSolves(Solutions &s,Question &q,VarWeight &vw,const int &threshold,const int &solution_num){
    s.var_num=q.var_num;
    if(q.var_num <= threshold){
        allSolves(q,s);
    } else {
        s.solution_num=solution_num;
        srand(time(0));
        for(int j=0;s.solution_set.size()<s.solution_num;++j){
            vector<int> tmp(q.var_num);
            for(int k=0;k<q.var_num;++k){
                double trans=(double)rand()/RAND_MAX;
                if(trans>vw.weight[k]){
                    tmp[k]=-k-1;
                }else{
                    tmp[k]=k+1;
                }
            }
            s.solution_set.insert(tmp);
            s.solutions.push_back(tmp);
        }
    }
}