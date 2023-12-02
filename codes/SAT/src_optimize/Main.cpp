#include <iostream>
#include <string>
#include <chrono>
#include<iomanip>
#include<fstream>
#include "SAT.h"
#include<stdlib.h>

using namespace std;
using namespace szx;


void loadInput(istream& is, SATProblem& gc) {
	char type,tmp[100000];
	is>>type;
	while(type=='c') {
		is.getline(tmp,100000);
		is>>type;
	}
	is>>tmp;
	is >> gc.num >> gc.clauseNum ;
	gc.C.resize(gc.clauseNum);
	for (auto &cln:gc.C) { 
		int t;is>>t;
		while(t!=0) {cln.push_back(t);is>>t;}
	}
	int m=gc.clauseNum;
	int n=gc.num;
	double r=m*1.0/n;
	cerr<<m<<endl;
	cerr<<n<<endl;
	cerr<<fixed<<setprecision(3)<<r<<endl;
}
void loadAssignmentPrefer(istream& is, SATProblem& gc){
	int num;
	is>>num;
	gc.assignmentPrefers.resize(num);
	double t;
	for(auto &assignmentPrefer:gc.assignmentPrefers){
		for(int i=0;i<gc.num;i++){
			is>>t;
			assignmentPrefer.push_back(t);
		}
	}
	
	if(gc.assignmentPrefers.size()==2){
		gc.perUpLimit={15,15};
	}
	else{
		gc.perUpLimit={30};
	}
	cerr<<gc.assignmentPrefers.size()<<endl;
}
void saveOutput(ostream& os, vector<VariableValue>& Xs,int varnum) {
	os<<"s"<<" "<<varnum<<" "<<Xs.size()<<endl;
	for(auto X:Xs){
		for(int i=1;i<=varnum;i++){
			if(X[i]) os<<i<<" ";
			else os<<-i<<" ";
		}
		os<<"0"<<endl;
	}
	// for (auto val = X.begin(); val != X.end(); ++val) { os << *val << endl; }
}
bool check(vector<VariableValue>& Xs,SATProblem& SAT){
	for(auto X:Xs){
		for(auto cln:SAT.C){
			bool flag=0;
			for(auto i:cln){
				int id=abs(i);
				flag|=(i<0?(1^X[id]):X[id]);
			}
			if(!flag) return false;

		}
	}
	
	return true;
}
void test(istream& inputStream, ostream& outputStream,istream& preferinputStream,  long long secTimeout, int randSeed,bool isHasAssignment,int solUpLimit) {
	// cerr << "load input." << endl;
	SATProblem SAT;
	loadInput(inputStream,SAT);
	if(isHasAssignment) {loadAssignmentPrefer(preferinputStream,SAT);}
	SAT.solUpLimit=solUpLimit;
	chrono::steady_clock::time_point endTime = chrono::steady_clock::now() + chrono::seconds(secTimeout);
	vector<VariableValue> variableValues(solUpLimit);
	for(auto &iter:variableValues){
		iter.resize(SAT.num+1);
	}
	clock_t start=clock();
	solveSATProblem(variableValues, SAT, [&]() -> bool { return endTime < chrono::steady_clock::now(); }, randSeed,isHasAssignment);
	clock_t end=clock();
	// cerr << "save output." << endl;
	// saveOutput(outputStream, variableValue);
	int flag=check(variableValues,SAT);

	if((endTime < chrono::steady_clock::now())) cerr<<"timeout"<<endl;
	else cerr<<"ans:"<<flag<<endl;

	saveOutput(outputStream, variableValues,SAT.num);
	// else cerr<<"ans:"<<<<endl;
	
}

int main(int argc, char* argv[]) {
	// cerr << "load environment." << endl;
	srand(time(0));
	if (argc ==2) {
		long long secTimeout = atoll(argv[1]);
		test(cin, cout, cin,secTimeout, static_cast<int>(time(nullptr) + clock()),0,1);
	} else if(argc==5){
		ifstream ifs(argv[1],ios::in);
		ofstream ofs(argv[2],ios::out);
		// ifstream vm(argv[3],ios::in);
		long long secTimeout = atoll(argv[3]);
		int solNum=atoi(argv[4]);
		test(ifs, ofs, ifs,secTimeout, static_cast<int>(time(nullptr) + clock()),0,solNum);
	}
	else if(argc==6){
		ifstream ifs(argv[1],ios::in);
		ofstream ofs(argv[2],ios::out);
		ifstream vm(argv[3],ios::in);
		long long secTimeout = atoll(argv[4]);
		int solNum=atoi(argv[5]);
		test(ifs, ofs, vm,secTimeout, static_cast<int>(time(nullptr) + clock()),1,solNum);
	}
	return 0;
}
