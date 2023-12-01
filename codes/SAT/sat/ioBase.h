//
// Create by casaji on 2023/4/27
//

#ifndef MAIN_CPP_IOBASE_H
#define MAIN_CPP_IOBASE_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "structBase.h"

void ReadFromFcnf(const std::string &Fcnf, Question &q);
void ReadFromFsolve(const std::string &Fsolve, Solutions &s);
void ReadOrCreateFvw(const std::string &Fvw, VarWeight &vw, int var_num);
//void ReadOrCreateRandomRvw(const std::string &Fvw, VarWeight &vw, int var_num);
void ReadFromFanswer(const std::string &Fanswer, Answer &asw);

void WriteToFsolve(const std::string &Fsolve, Solutions &s);
void WriteToFanswer(const std::string &Fasw, Answer &asw);
void WriteToFvw(const std::string &Fvw, VarWeight &vw);

#endif // MAIN_CPP_IOBASE_H