//
// Create by casaji on 2023/4/18
//

#ifndef MAIN_CPP_WALKSAT_H
#define MAIN_CPP_WALKSAT_H

#include <string>
#include <vector>

typedef struct cs{
    int clause_num;
    int var_num;
    std::vector<int> clause_sat_times;
    std::vector<std::vector<int>> var_covered_list;
}walksat;

void WalkSat(const std::string &Fcnf, const std::string &Fsolve,
             const std::string &Fvw, const int &threshold, const int &solution_num);
// void WalkSat(const std::string &Fcnf, const std::string &Fsolve,
//              const std::string &Fvw, const std::string &Fasw,
//              std::vector<int> &revn, const int &threshold, const int &solution_num);

#endif // MAIN_CPP_WALKSAT_H