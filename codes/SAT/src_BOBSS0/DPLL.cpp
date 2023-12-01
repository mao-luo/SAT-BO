#include "SAT.h"

#include <random>
#include <iostream>
#include<queue>
#include <algorithm>
using namespace std;

namespace szx
{

	class Solver
	{
		// random number generator.
		mt19937 pseudoRandNumGen;
		void initRand(int seed) { pseudoRandNumGen = mt19937(seed); }
		int fastRand(int lb, int ub) { return (pseudoRandNumGen() % (ub - lb)) + lb; }
		int fastRand(int ub) { return pseudoRandNumGen() % ub; }
		int rand(int lb, int ub) { return uniform_int_distribution<int>(lb, ub - 1)(pseudoRandNumGen); }
		int rand(int ub) { return uniform_int_distribution<int>(0, ub - 1)(pseudoRandNumGen); }
		struct Node{
			VariableId name;
			int assignmentPrefer;
			Clause S[2],id;
			inline bool isEmpty(){
				return ((!S[0][0])&&(!S[1][0]));
			}
			inline void init(ClauseId cln,VariableId nameInit){
				name=nameInit;
				S[0].push_back(0);
				S[1].push_back(0);
				id.resize(cln);
			}
			inline bool addS(ClauseId clid,bool val){
				S[val].push_back(clid);
				++S[val][0];
				id[clid]=S[val][0];
			}
			inline void removeS(ClauseId clid,bool val){
				S[val][id[clid]]=S[val].back();
				id[S[val].back()]=id[clid];
				id[clid]=0;
				--S[val][0];
				S[val].pop_back();
			}	
			inline void virtualRemoveS(ClauseId clid,LiteralId liter){
				bool val=(liter>0);
				VariableId en=S[val][S[val][0]];
				swap(id[clid],id[en]);
				swap(S[val][id[clid]],S[val][id[en]]);
				S[val][0]--;
			}
			inline void virtualAddS(LiteralId liter){
				S[liter>0][0]++;
			}
		};
		struct ClauseNode{
			Clause C,id;
			VariableId len=0;
			inline bool createClause(Clause clause,VariableId n){
				C.clear();C.push_back(0);len=0;
				id.clear();
				id.resize(n+1,0);
				for(auto iter:clause){
					int idName=getIterId(iter);
					if(id[idName]){
						if(C[id[idName]]==iter) continue;
						else {
							return false;
						}
					}
					len++;
					id[idName]=len;
					C.push_back(iter);
				}
				return true;
			}
			inline bool isUnitClause(){
				return (len==1);
			}
			inline bool isEmpty(){
				return (len==0);
			}
			inline bool removeLiteral(LiteralId liter){
				LiteralId en=getIterId(C[len]);
				liter=getIterId(liter);
				C[id[liter]]=C[len];
				id[en]=id[liter];
				len--;
				C.pop_back();
			}
			inline int getIterId(LiteralId x){
				return (x^(x>>31))-(x>>31);;
			}
			inline void virtualRemoveLiter(LiteralId x){
				VariableId en=getIterId(C[len]);
				x=getIterId(x);
				swap(id[x],id[en]);
				swap(C[id[x]],C[id[en]]);
				len--;
			}
			inline void virtualAddLiter(){
				len++;
			}
	
		};
		struct sol{
			vector<bool> pos,ans;
			vector<VariableId> vis;
			vector<ClauseNode> F;
			vector<Node> X;
			vector<VariableId> X_id;
			vector<vector<ClauseId>> bucketOfFLen;
			vector<ClauseId> bucketId;
			ClauseId F_len=0;
			VariableId X_len=0;
			VariableId n,MinLen;
			ClauseId cln;
			vector<LiteralId> isSearch;
			bool isHasEmptyClause=false;
			int T=10,T_MAX=100000;
			const int basicLen=2;
			inline bool insert(VariableId &len,ClauseId clid){
				bucketId[clid]=bucketOfFLen[len].size();
				bucketOfFLen[len].push_back(clid);
			}
			inline bool remove(VariableId &len,ClauseId clid){
				vector<int> &bucket=bucketOfFLen[len];
				int clidEnd=bucket.back();
				bucketId[clidEnd]=bucketId[clid];
				bucket[bucketId[clid]]=clidEnd;
				bucket.pop_back();
			}
			inline int getIterId(LiteralId x){
				return (x^(x>>31))-(x>>31);;
			}
			inline bool isSatisfied(){
				return (F_len==0);
			}
			inline void addQ(LiteralId x,vector<LiteralId> &decisionLevel){
				VariableId id=getIterId(x);
				if(pos[id]) return ;
				pos[id]=true;
				decisionLevel.push_back(x);
			}
			inline bool InitialSpaceAllocation(SATProblem& input){
				cln=input.clauseNum;//子句数量
				n=input.num;//变元数量
				if(cln*1.0/n<3) T_MAX=600;else T_MAX=300;//单元子句传播上限
				ans.resize(n+1);pos.resize(n+1,0),vis.resize(cln,0);
				X_id.resize(n+1),X.resize(n+1);
				bucketOfFLen.resize(n+1),F.resize(cln),bucketId.resize(cln);
				X_len=n;F_len=0;
				isSearch.resize(n+1,0);
				for(int i=0;i<n;i++) {X_id[i]=i+1;X[i+1].init(cln,i);}
			}
			inline bool init(SATProblem& input,std::vector<double> &assignmentPrefer){
				InitialSpaceAllocation(input);
				vector<int> decisionLevel;
				for(int i=0;i<cln;i++){
					Clause clause=input.C[i];
					ClauseId clid=F_len;
					ClauseNode &cl=F[clid];
					if(!cl.createClause(clause,n)) continue;
					if(cl.len==1) addQ(cl.C[1],decisionLevel);
					insert(cl.len,clid);
					for(int j=1;j<=cl.len;j++){
						int &liter=cl.C[j];
						getNode(liter).addS(clid,(liter>0));
					}
					F_len++;
				}
				for(int i=0;i<X_len;i++){
					int x=X_id[i];
					Node &node=X[x];
					
				}
				if(assignmentPrefer.size()){
					for(int i=0;i<n;i++){
						X[i+1].assignmentPrefer=assignmentPrefer[i]*10;
						if(assignmentPrefer[i]>0.5) ans[i]=1;
						else ans[i]=0;
					}
				}
				vector<int> tmp;
				if(!unit_propagation(decisionLevel,tmp))return false;
				decisionLevel.clear();
				
				// for(int i=0;i<X_len;i++) if(X[X_id[i]].isEmpty()) decisionLevel.push_back(X_id[i]);
				// for(auto i:decisionLevel) vitrualRemoveNode(i);
				return true;
			}
			inline bool init(SATProblem& input){
				InitialSpaceAllocation(input);
				vector<int> decisionLevel;
				for(int i=0;i<cln;i++){
					Clause clause=input.C[i];
					ClauseId clid=F_len;
					ClauseNode &cl=F[clid];
					if(!cl.createClause(clause,n)) continue;
					if(cl.len==1) addQ(cl.C[1],decisionLevel);
					insert(cl.len,clid);
					for(int j=1;j<=cl.len;j++){
						int &liter=cl.C[j];
						getNode(liter).addS(clid,(liter>0));
					}
					F_len++;
				}
				for(int i=0;i<X_len;i++){
					int x=X_id[i];
					Node &node=X[x];
					
				}
				vector<int> tmp;
				if(!unit_propagation(decisionLevel,tmp))return false;
				decisionLevel.clear();
				
				// for(int i=0;i<X_len;i++) if(X[X_id[i]].isEmpty()) decisionLevel.push_back(X_id[i]);
				// for(auto i:decisionLevel) vitrualRemoveNode(i);
				return true;
			}
			inline void vitrualRemoveNode(LiteralId liter){
				liter=getIterId(liter);
				X_len--;
				LiteralId en=X_id[X_len];
				swap(X_id[X[liter].name],X_id[X_len]);
				swap(X[liter].name,X[en].name);	
			}
			inline void vitrualAddNode(){
				X_len++;			
			}
			inline Node& getNode(LiteralId &x){
				return X[getIterId(x)];
			}
			inline void getAns(vector<bool> &output){
				output=ans;
			}
			inline void removeSatisfiedClauses(Clause &S,LiteralId &liter,vector<LiteralId> &q,vector<ClauseId> &changeClauseSet){
				for(int i=1;i<=S[0];i++){
					ClauseId &clause=S[i];
					ClauseNode &cl=F[clause];
					if(!vis[clause]) {vis[clause]=cl.len*-1;changeClauseSet.push_back(clause);}
					else if(vis[clause]>0) vis[clause]*=-1;
					for(int j=1;j<=cl.len;j++){
						LiteralId x=cl.C[j];
						if(x==liter) continue;
						Node &node=getNode(x);
						node.virtualRemoveS(clause,(x>0));
						
					}
				}
				F_len-=S[0];
			}
			inline void removeUnsatisfiedClauses(Clause &S,LiteralId liter,vector<LiteralId> &q,vector<ClauseId> &changeClauseSet){
				for(int i=1;i<=S[0];i++){
					ClauseNode &cl=F[S[i]];
					if(!vis[S[i]]) {vis[S[i]]=cl.len;changeClauseSet.push_back(S[i]);}
					cl.virtualRemoveLiter(liter);
					if(cl.len==0) isHasEmptyClause=true;
					if(cl.len==1&&!pos[getIterId(cl.C[1])]){ q.push_back(cl.C[1]);pos[getIterId(cl.C[1])]=1;}
				}
			}
			inline void addSatisfiedClauses(Clause &S,LiteralId &liter){
				F_len+=S[0];
				for(int i=S[0];i>=1;i--){
					ClauseId &clause=S[i];
					ClauseNode &cl=F[clause];
					for(int j=cl.len;j>=1;j--){
						LiteralId &x=cl.C[j];
						if(x==liter) continue;
						getNode(x).virtualAddS(x);
					}
				}
			}
			inline void addUnsatisfiedClauses(Clause &S){
				for(int i=S[0];i>=1;i--){
					ClauseNode &cl=F[S[i]];
					cl.virtualAddLiter();
				}
			}
			bool unit_propagation(vector<LiteralId> &decisionLevel,vector<ClauseId> &changeClauseSet){
				if(!single_unit_propagation(decisionLevel,changeClauseSet))return false;
				for(auto i:decisionLevel) {vitrualRemoveNode(i);ans[getIterId(i)]=(i>0);}
				for(auto clid:changeClauseSet){
					int originLen=abs(vis[clid]);
					remove(originLen,clid);
					ClauseNode &cl=F[clid];
					if(vis[clid]<0) continue;
					insert(cl.len,clid);
					vis[clid]=0;
				}
				return true;
			}
			void recovery(vector<LiteralId> &decisionLevel,vector<ClauseId> &changeClauseSet){
				for(auto i:decisionLevel) vitrualAddNode();
				for(auto i:changeClauseSet) if(vis[i]==0) remove(F[i].len,i);
				singleRecovery(decisionLevel,changeClauseSet);
				for(auto i:changeClauseSet) insert(F[i].len,i);
			}
			bool single_unit_propagation(vector<LiteralId> &decisionLevel,vector<ClauseId> &changeClauseSet){
				for(auto i:decisionLevel) pos[getIterId(i)]=true;
				for(int i=0;i<decisionLevel.size();i++){
					LiteralId liter=decisionLevel[i];
					Node &node=getNode(liter);
					removeSatisfiedClauses(node.S[liter>0],liter,decisionLevel,changeClauseSet);
					removeUnsatisfiedClauses(node.S[liter<0],liter,decisionLevel,changeClauseSet);
					if(isHasEmptyClause) {
						isHasEmptyClause=false;
						while(decisionLevel.back()!=liter){ pos[getIterId(decisionLevel.back())]=false;	decisionLevel.pop_back();}
						singleRecovery(decisionLevel,changeClauseSet);
						return false;
					}
				}
				return true;
			}		
			void singleRecovery(vector<LiteralId> &decisionLevel,vector<ClauseId> &changeClauseSet){
				reverse(decisionLevel.begin(),decisionLevel.end());
				for(auto liter:decisionLevel){
					Node &node=getNode(liter);
					pos[getIterId(liter)]=false;
					addUnsatisfiedClauses(node.S[liter<0]);
					addSatisfiedClauses(node.S[liter>0],liter);
				}
				for(auto i:changeClauseSet) vis[i]=0;
			}
			void getPreferAssignment(vector<VariableId> &can){
				int mx=0;
				for(int i=0;i<X_len;i++){
					Node &node=getNode(X_id[i]);
					int tmp=abs(node.assignmentPrefer-5);
					if(tmp>mx){
						mx=tmp;
						can.clear();
						if(node.assignmentPrefer>5) can.push_back(X_id[i]);
						else can.push_back(-X_id[i]);
					}
					else if(tmp==mx){
						if(node.assignmentPrefer>5) can.push_back(X_id[i]);
						else can.push_back(-X_id[i]);
					}
				}
			}
			void getCandicate(vector<VariableId> &can){
				if(F_len==0){
					can.push_back(X_id[0]);
					return ;
				}
				for(MinLen=2;MinLen<=n;MinLen++) if(bucketOfFLen[MinLen].size())break;
				
				vector<int> tmp(X_len,0);
				int mx=0;
				for(int len=MinLen;len<=max(MinLen,min(3,(int)(bucketOfFLen.size()-1)));len++){
					int bas=(len==2?5:1);
					for(auto clid:bucketOfFLen[len]){
						ClauseNode &cl=F[clid];
						for(int j=1;j<=len;j++){
							LiteralId i=getNode(cl.C[j]).name;
							
							tmp[i]+=bas;
						}
					}
				}
				for(int i=0;i<X_len;i++) mx=max(mx,tmp[i]);
				for(int i=0;i<X_len;i++) if(mx==tmp[i])can.push_back(X_id[i]);
			}	
			vector<LiteralId> decisionLevl;
			
		};
		int getlevel(sol &ans,int num,int allnum){
				int prelevel=ans.decisionLevl.size();
				vector<int> nullSearch;
				int j=0;
				vector<int> level;
				for(auto x:ans.decisionLevl){
					++j;
					if(ans.isSearch[x]) continue;
					nullSearch.push_back(x);
					level.push_back(j);
				}
				if(2*num>allnum&&nullSearch.size()>200){
					int p=rand(100)+1;
					if(p<10*num/allnum){
						prelevel=level[rand(level.size())];
					}
				}
				return prelevel;
		}
		long long NodeNums=0;
		long long outputs=0;
		int limit_l=0,limit;
		bool dpll(sol &ans,vector<int> q,vector<VariableValue>& output,std::function<bool()> isTimeout){
			NodeNums++;
			vector<int> tmp;
			if(isTimeout()){return false;}
			if(!ans.unit_propagation(q,tmp)){return false;}
			if(ans.isSatisfied()){
				for(int i=0;i<ans.X_len;i++){
					int id=ans.X_id[i];
					Node &node=ans.X[id];
					ans.ans[id]=rand(100)%2;
				}
				ans.getAns(output[outputs]);
				ans.recovery(q,tmp);
				outputs++;
				limit_l++;
				if(limit_l==limit)return true;
				return false;
			}
			vector<VariableId> can;
			for(auto &i:can){
				if(rand(100)<50) i*=-1;
			}
			ans.getCandicate(can);
			VariableId x=can[rand(can.size())];
			if(dpll(ans,{x},output,isTimeout)||dpll(ans,{-x},output,isTimeout)) return true;
			ans.recovery(q,tmp);
			return false;
		}
		int preLevel=0;
		int haspre;
		bool dpllForAssignment(sol &ans,vector<int> q,vector<VariableValue>& output,std::function<bool()> isTimeout){
			NodeNums++;
			vector<int> tmp;
			if(isTimeout()){return false;}
			if(!ans.unit_propagation(q,tmp)){
				// preLevel=getlevel(ans,limit_l,limit);
				return false;
			}
			if(ans.isSatisfied()){
				// for(int i=0;i<ans.X_len;i++){
				// 	int id=ans.X_id[i];
				// 	Node &node=ans.X[id];
				// 	if(rand(100)<20)
				// 		ans.ans[id]=rand(100)%2;
				// 	else{
				// 		ans.ans[id]=(node.assignmentPrefer>5?1:0);
				// 	}
				// }
				ans.getAns(output[outputs]);
				ans.recovery(q,tmp);
				outputs++;
				limit_l++;
				if(limit_l==limit)return true;
				return false;
			}
			vector<VariableId> can;
			ans.getPreferAssignment(can);
			VariableId x=can[rand(can.size())];
			// double perheps=(5.0/max(0.5,1.0*abs(ans.getNode(x).assignmentPrefer-5)));
			// int pp=rand(100)+1;
			// // cerr<<perheps<<" "<<pp<<" "<<ans.getNode(x).assignmentPrefer<<endl;
			// if(pp<perheps){
			// 	x=-x;

			// 	// cerr<<perheps<<" "<<pp<<" "<<ans.getNode(x).assignmentPrefer<<endl;
			// }
			// ans.decisionLevl.push_back(abs(x));
			if(dpllForAssignment(ans,{x},output,isTimeout)) return true;
			
			if(preLevel&&preLevel!=ans.decisionLevl.size()) {
				// cerr<<"ok"<<endl;
				// cerr<<preLevel<<" "<<ans.decisionLevl.size()<<endl;
				// ans.decisionLevl.pop_back();
				ans.recovery(q,tmp);
				return false;
			}
			// if(preLevel==ans.decisionLevl.size()){
			// 	// cerr<<"okk"<<endl;
			// }
			// ans.isSearch[abs(x)]=true;
			// preLevel=0;
			if(dpllForAssignment(ans,{-x},output,isTimeout)) return true;
			// if(preLevel==ans.decisionLevl.size()){
			// 	preLevel=0;
			// }
			// ans.decisionLevl.pop_back();
			// ans.isSearch[abs(x)]=false;
			ans.recovery(q,tmp);
			// if(!preLevel) {
			// 	preLevel=getlevel(ans,limit_l,limit);
			// }
			// cerr<<"ok:"<<(preLevel)<<" "<<ans.decisionLevl.size()<<endl;;
			return false;
		}
		sol ansers[3];
	public:
		//递归改为非递归
		void solve(vector<VariableValue>& output, SATProblem& input, std::function<bool()> isTimeout, int seed,bool isHasAssignmentPrefer)
		{
			NodeNums=0;
			initRand(seed);
			limit_l=0;
			if(isHasAssignmentPrefer){ 
				
				int i=0;
				for(auto assignmentPrefer:input.assignmentPrefers){
					limit=input.perUpLimit[i];
					limit_l=0;
					sol &ans=ansers[i];
					if(!ans.init(input,assignmentPrefer)) {cerr<<NodeNums<<endl;cerr<<outputs<<endl;return ;}
					dpllForAssignment(ans,{},output,isTimeout);
					i++;

				}
			}
			else {
				limit=input.solUpLimit;
				sol &ans=ansers[0];
				if(!ans.init(input)) {cerr<<NodeNums<<endl;cerr<<outputs<<endl;return ;}
				dpll(ans,{},output,isTimeout);
			}
			cerr<<NodeNums<<endl;
			cerr<<outputs<<endl;
			while(output.size()>outputs) output.pop_back();
		}
	};

	// solver.
	void solveSATProblem(vector<VariableValue>& output, SATProblem& input, std::function<bool()> isTimeout, int seed,bool isHasAssignmentPrefer)
	{
		Solver().solve(output, input, isTimeout, seed,isHasAssignmentPrefer);
		
		// cerr << "end\n";
	}

}
