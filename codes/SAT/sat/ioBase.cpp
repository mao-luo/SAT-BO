#include "ioBase.h"

#include <ctime>
#include <iomanip>

using namespace std;

void ReadFromFcnf(const string &Fcnf, Question &q){
    ifstream fcnf(Fcnf, ios::in);
    string tmp;
    fcnf>>tmp>>tmp>>q.var_num>>q.clause_num;
    q.clause.resize(q.clause_num);
    int i=0;
    while(i<q.clause_num){
        int t;
        fcnf >>t;
        q.clause[i].push_back(t);
        if(t==0){
            ++i;
        }
    }
    fcnf.close();
}

void ReadFromFsolve(const string &Fsolve, Solutions &s){
    ifstream fsolve(Fsolve, ios::in);
    string tmp;
    fsolve>>tmp;
    if(tmp!="s"){
        cerr<<"This file is not belong to solve-file!"<<endl;
        cerr<<tmp<<endl;
        fsolve>>tmp;
        cerr<<tmp<<endl;
        return;
    }
    fsolve>>s.var_num;
    fsolve>>s.solution_num;
    s.solutions.resize(s.solution_num);
    int tint;
    for(int j=0;j<s.solution_num;++j){
        fsolve>>tint;
        while(tint!=0){
            s.solutions[j].push_back(tint);
            fsolve>>tint;
        }
    }
    fsolve.close();
}

void ReadOrCreateFvw(const string &Fvw, VarWeight &vw, int var_num){
    vw.var_num=var_num;
    vw.weight.resize(vw.var_num,0.5);
    ifstream ifvw(Fvw,ios::in);
    if(!ifvw.is_open()){
        ofstream ofvw(Fvw,ios::out);
        for(int j=0;j<var_num;++j){
            ofvw<<vw.weight[j]<<"\n";
        }
        ofvw.close();
    } else {
        double tmp;
        for(int j=0;j<var_num;++j){
        ifvw>>tmp;
        vw.weight[j]=tmp;
        }
    }
    ifvw.close();
}

//void ReadOrCreateRandomRvw(const string &Fvw, VarWeight &vw, int var_num){}

void ReadFromFanswer(const string &Fanswer, Answer &asw){
    ifstream fasw(Fanswer, ios::in);
    string flag;
    fasw>>flag;
    if(flag!="asw"){
        cerr<<"This file is not belong to feedback-file!"<<endl;
        return;
    }
    fasw>>asw.solution_num;
    asw.solutions.resize(asw.solution_num);
    asw.feedback.resize(asw.solution_num);
    int tint;
    for(int j=0;j<asw.solution_num;++j){
        fasw>>tint;
        while(tint!=0){
            asw.solutions[j].push_back(tint);
            fasw>>tint;
        }
        fasw>>tint;
        asw.feedback[j]=tint;
    }
    fasw.close();
}

void WriteToFsolve(const string &Fsolve, Solutions &s){
    ofstream fsolve(Fsolve, ios::out);
    fsolve<<"s "<<s.var_num<<" "<<s.solution_num<<"\n";
    for(int i=0;i<s.solution_num;++i){
        for(size_t j=0;j<s.solutions[i].size();++j){
            fsolve<<s.solutions[i][j]<<" ";
        }
        fsolve<< "0 \n";
    }
    fsolve.close();
}

void WriteToFanswer(const string &Fasw, Answer &asw){
    ofstream fasw(Fasw,ios::out);
    fasw<<"asw "<<asw.solution_num<<"\n";
    for(int i=0;i<asw.solution_num;++i){
        for(size_t j=0;j<asw.solutions[i].size();++j){
            fasw<<asw.solutions[i][j]<<" ";
        }
        fasw<<"0 "<<asw.feedback[i]<<endl;
    }
    fasw.close();
}

void WriteToFvw(const string &Fvw, VarWeight &vw){
    ofstream ofvw(Fvw, ios::out);
    for(int i=0;i<vw.var_num;++i){
        ofvw<<vw.weight[i]<<"\n";
    }
    ofvw.close();
}