//
// Create by casaji on 2023/4/18
//

#include "walksat.h"

#include <ctime>
#include <fstream>
#include <iostream>

//#include "check.h"
#include "dpll.h"
#include "genSolutions.h"
#include "ioBase.h"

using namespace std;

void InitWalkSat(const string &Fcnf, const string &Fsolve, const string &Fvw,
                 Question &q,Solutions &s,walksat &w, VarWeight &vw,
                 const int &threshold, const int &solution_num) {
    // read fcnf
    ReadFromFcnf(Fcnf, q);
    // init fsolve
    ofstream fsolve(Fsolve, ios::out);
    fsolve << "s " << 0;
    fsolve.close();
    // init fvw
    ReadOrCreateFvw(Fvw, vw, q.var_num);
    // init solve
    InitSolves(s,q,vw,threshold,solution_num);
    // init walksat w
    w.clause_num=q.clause_num;
    w.var_num=q.var_num;
    w.var_covered_list.resize(2*w.var_num+1);
    for(size_t j=0;j<w.var_covered_list.size();++j){
        vector<int> tmp(0);
        w.var_covered_list[j] =tmp;
    }
}

void ResetWalkSat(walksat &w){
    w.clause_sat_times.clear();
    w.clause_sat_times.resize(w.clause_num,0);
}

void FillWithClauseSatTime(walksat &w,Question &q, vector<int> &s){
    for(int i=0;i<q.clause_num;++i){
        for(size_t j=0;j<q.clause[i].size();++j){
            if(q.clause[i][j]>0){
                if(q.clause[i][j]==s[q.clause[i][j] - 1]){
                    ++w.clause_sat_times[i];
                }
            } else if (q.clause[i][j] <0){
                if(q.clause[i][j]==s[-q.clause[i][j] - 1]){
                    ++w.clause_sat_times[i];
                }
            } else{
                break;
            }
        }
    }
}

void FillWithVarCoveredList(walksat &w,Question &q){
    for(int i=0;i<q.clause_num;++i){
        for(size_t j=0;j<q.clause[i].size();++j){
            if(q.clause[i][j]!=0){
                w.var_covered_list[q.clause[i][j]+w.var_num].push_back(i);
            }
        }
    }
}

void LocalSearch(walksat &w,Question &q, vector<int> &s, int max_count){
    int FX=0;
    vector<int> unsat_clause;
    for(int i=0;i<w.clause_num;++i){
        if(w.clause_sat_times[i]==0){
            ++FX;
            unsat_clause.push_back(i);
        }
    }
    int index=0;
    vector<int> clause_sat_times_now =w.clause_sat_times;
    vector<int> clause_sat_times_last(0);
    vector<int> clause_sat_times_best(0);
    while(FX!=0&&index<max_count){
        ++index;
        int rand_unsat_clause = rand()%unsat_clause.size();
        int rand_clause = unsat_clause[rand_unsat_clause];
        int delta_max=0;
        int ind_max=0;
        int delta_now=0;
        clause_sat_times_last=clause_sat_times_now;
        for(size_t i=0;i<q.clause[rand_clause].size();++i){
            int var_open = q.clause[rand_clause][i] + w.var_num;
            int var_close = w.var_num - q.clause[rand_clause][i];
            int delta_open = 0, delta_close = 0;
            for(size_t j=0;j<w.var_covered_list[var_open].size();++j){
                if(clause_sat_times_now[w.var_covered_list[var_open][j]]==0)
                    ++delta_open;
                ++clause_sat_times_now[w.var_covered_list[var_open][j]];
            }
            for(size_t j=0;j<w.var_covered_list[var_close].size();++j){
                if(clause_sat_times_now[w.var_covered_list[var_close][j]]==0)
                    ++delta_close;
                --clause_sat_times_now[w.var_covered_list[var_close][j]];
            }
            delta_now=delta_open-delta_close;
            if(delta_max<delta_now){
                delta_max=delta_now;
                ind_max=var_open;
                clause_sat_times_best=clause_sat_times_now;
            }
            clause_sat_times_now=clause_sat_times_last;
        }
        FX=FX-delta_max;
        if(FX<0){
            cerr<<"fatal error!"<<endl;
        }
        unsat_clause.clear();
        for(int i=0;i<w.clause_num;++i){
            if(clause_sat_times_best[i]==0){
                unsat_clause.push_back(i);
            }
        }
        int var_index = ind_max<w.var_num?w.var_num-ind_max:ind_max-w.var_num;
        s[var_index - 1]=-s[var_index - 1];
        w.clause_sat_times=clause_sat_times_best;
        clause_sat_times_now=w.clause_sat_times;
    }
    if(FX!=0){
        cout<<"Failed! Index is "<<index<<endl;
    }
    return;
}

void WalkSat(const string &Fcnf, const string &Fsolve, const string &Fvw,const int &threshold=10,const int &solution_num=100){
    Question q;
    Solutions s;
    walksat w;
    VarWeight vw;
    InitWalkSat(Fcnf,Fsolve,Fvw,q,s,w,vw,threshold,solution_num);
    FillWithVarCoveredList(w,q);
    int count=s.solution_num;
    for(int i=0;i<count;++i){
        ResetWalkSat(w);
        FillWithClauseSatTime(w,q,s.solutions[i]);
        LocalSearch(w,q,s.solutions[i],solution_num);
    }
    WriteToFsolve(Fsolve,s);
    return;
}

// void WalkSat(const string &Fcnf, const string &Fsolve, const string &Fvw,
//              const string &Fasw, vector<int> &revn, const int &threshold=10,
//              const int &solution_num=100){
//     Question q;
//     Solutions s;
//     walksat w;
//     VarWeight vw;
//     Answer asw;
//     InitWalkSat(Fcnf,Fsolve,Fvw,q,s,w,vw,threshold,solution_num);
//     FillWithVarCoveredList(w,q);
//     int count=s.solution_num;
//     for(int i=0;i<count;++i){
//         ResetWalkSat(w);
//         FillWithClauseSatTime(w,q,s.solutions[i]);
//         LocalSearch(w,q,s.solutions[i],solution_num);
//     }
//     WriteToFsolve(Fsolve,s);
//     CalcAnswer(s, asw,revn);
//     WriteToFanswer(Fasw,asw);
//     return;
// }