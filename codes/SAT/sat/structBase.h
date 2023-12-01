//
// Create by casaji on 2023/4/27
//

#ifndef MAIN_CPP_STRUCTBASE_H
#define MAIN_CPP_STRUCTBASE_H

#include <set>
#include <vector>

typedef struct qt {
    int var_num;
    int clause_num;
    std::vector<std::vector<int>> clause;
    //std::vector<int> map;
} Question;

typedef struct tree {
    int node_id;
    int clause_num;
    std::vector<std::vector<int>> clause;
    std::vector<int> solve;
    struct tree *left;
    struct tree *right;
    struct tree *parent;
    tree(int ni,int cn)
        : node_id(ni),
          clause_num(cn),
          left(nullptr),
          right(nullptr),
          parent(nullptr){}
}SolveTree;
    
struct Solutions{
    int var_num;
    int solution_num;
    std::vector<std::vector<int>> solutions;
    std::set<std::vector<int>> solution_set;
    Solutions(){};
    Solutions(int var_num, int solution_num)
        : var_num(var_num), solution_num(solution_num){
        solutions.resize(solution_num);
    };
};

typedef struct vw{
    int var_num;
    std::vector<float> weight;
    vw(){};
    vw(int vn):var_num(vn),weight(vn){};
    vw(int vn,int w):var_num(vn),weight(vn,w){};
} VarWeight;

struct Answer{
    int solution_num;
    std::vector<std::vector<int>> solutions;
    std::vector<int> feedback;
};

#endif // MAIN_CPP_STRUCTBASE_H