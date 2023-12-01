#include "check.h"

#include <fstream>
#include <iostream>

#include "dpll.h"
#include "ioBase.h"

using namespace std;

void CheckFunc(Question &q,Solutions &s,int &result,set<int>&conflict_solve, vector<int>&conflict_var){
    for(int i=0;i<s.solution_num;++i){
        for (int j = 0; j < q.clause_num; ++j){
            for(size_t k=0;k<q.clause[j].size();++k){
                if(q.clause[j][k]>0){
                    if(q.clause[j][k]==s.solutions[i][q.clause[j][k]-1]) break;
                }else if(q.clause[j][k]<0){
                    if(q.clause[j][k]==s.solutions[i][-q.clause[j][k]-1]) break;
                }else{
                    ++result;
                    conflict_solve.insert(i);
                    conflict_var.push_back(q.clause[j][k]);
                    break;
                }
            }
        }        
    }
}

void CheckSolve(const string &Fcnf, const string &Fsolve){
    Question q;
    ReadFromFcnf(Fcnf,q);
    Solutions s;
    ReadFromFsolve(Fsolve,s);
    int r=0;
    set<int> conflict_solve;
    vector<int> conflict_var;
    CheckFunc(q,s,r,conflict_solve,conflict_var);
    if(r==0){
        cout<<"Solve is right!"<<endl;
    }else{
        cout<<"Solve is wrong! Conflict num is "<<r<<endl;
    }
}