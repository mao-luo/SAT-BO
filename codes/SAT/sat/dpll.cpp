#include "dpll.h"

#include <ctime>
#include <iostream>
#include <stack>

using namespace std;

clock_t start=clock();
long long int solution_num=0;
int min_dp;
bool can_be_solved;
int count;
int count_index;
set<int> conflict;
int fixed_index, unfixed_index;

bool isMaySatisfiable(SolveTree *s){
    for(int i=0;i<s->clause_num;++i){
        if(s->clause[i].size()==1&&s->clause[i][0]==0){
            cout<<s->node_id<<": conflict!\n";
            return false;
        }
    }
    return true;
}

void SetChildTree(SolveTree *f,SolveTree *c,int value){
    c->clause=f->clause;
    c->parent=f;
    c->solve=f->solve;
    c->solve.push_back(value);
    if(value==1){
        for(int i=0;i<c->clause_num;++i){
            for(size_t j=0;i<c->clause_num&&j<c->clause[i].size();++j){
                if(c->clause[i][j]==f->node_id){
                    int index=i;
                    c->clause.erase(begin(c->clause)+i);
                    --c->clause_num;
                    i=index;
                    j=-1;
                }
                if(i<c->clause_num&&j>=0&&
                   c->clause[i][j]+f->node_id==0){
                    c->clause[i].erase(begin(c->clause[i])+j);
                    --j;
                }
            }
        }
        f->left=c;
    }else{
        for(int i=0;i<c->clause_num;++i){
            for(size_t j=0;i<c->clause_num&&j<c->clause[i].size();++j){
                if(c->clause[i][j]+f->node_id==0){
                    int index=i;
                    c->clause.erase(begin(c->clause)+i);
                    --c->clause_num;
                    i=index;
                    j=-1;
                }
                if(i<c->clause_num&&j>=0&&
                   c->clause[i][j]==f->node_id){
                    c->clause[i].erase(begin(c->clause[i])+j);
                    --j;
                }
            }
        }
        f->right=c;
    }
    return;
}

void Loop(SolveTree *s, int deep, int &var_num,vector<vector<int>> &result){
    if(deep>var_num) return;
    if(isMaySatisfiable(s)){
        SolveTree sl=SolveTree(s->node_id+1,s->clause_num);
        SolveTree *left=&sl;
        SetChildTree(s,left,1);
        SolveTree *p=s->left;
        Loop(p,deep+1,var_num,result);
        if(deep==var_num){
            result.push_back(s->solve);
        }
        SolveTree sr=SolveTree(s->node_id+1,s->clause_num);
        SolveTree *right=&sr;
        SetChildTree(s,right,0);
        p=s->right;
        Loop(p,deep+1,var_num,result);
    }
    return;
}

void allSolves(Question &q,Solutions &s){
    SolveTree st=SolveTree(1,q.clause_num);
    SolveTree *root;
    root=&st;
    min_dp=q.var_num;
    root->clause=q.clause;
    Loop(root,0,q.var_num,s.solutions);
    s.solution_num=s.solutions.size();
    for(int i=0;i<s.solution_num;++i){
        for(int j=0;j<s.var_num;++j){
            if(s.solutions[i][j]==0){
                s.solutions[i][j]=-j-1;
            }else{
                s.solutions[i][j]=j+1;
            }
        }
    }
    cout<<"solve_num : "<<solution_num+s.solution_num<<endl;
}