////////////////////////////////
/// usage : 1.	SDK for graph coloring solver.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_SZX_NPBENCHMARK_GRAPH_COLORING_H
#define CN_HUST_SZX_NPBENCHMARK_GRAPH_COLORING_H


#include <array>
#include <vector>
#include <functional>


namespace szx {

using VariableId = int;
using ClauseId=int;
using LiteralId = VariableId;

using Clause = std::vector<LiteralId>; // undirected link.

struct SATProblem {
	VariableId num;
	ClauseId clauseNum;
	LiteralId solUpLimit=1;
	std::vector<LiteralId> perUpLimit;
	LiteralId p;
	std::vector<Clause> C;
	std::vector<std::vector<double>> assignmentPrefers;
};
using VariableValue = std::vector<bool>; // undirected link.

void solveSATProblem(std::vector<VariableValue>& output, SATProblem& input, std::function<bool()> isTimeout, int seed,bool isHasAssignmentPrefer);

}


#endif // CN_HUST_SZX_NPBENCHMARK_GRAPH_COLORING_H
