#include "creatFlow.h"
#include<cmath>
#include <fstream>
#include <iostream>
#include<ctime>
#include<map>
#include<algorithm>
#include "ioBase.h"
using namespace std;
// int getXId(int x,int var_num){
//     return x+var_num;
// }
// /home/hyw/workspace/DatabaseAttackAndVerification/tu-rbowrapper-master/build/sat_test --creatFlow /home/hyw/workspace/DatabaseAttackAndVerification/specification_data/bd317aa5-b025-411f-b3bf-911b5ea83270/e4b68079-faad-4dde-ad77-771d7b962bb5.cnf /home/hyw/workspace/DatabaseAttackAndVerification/tu-rbowrapper-master/vitrualFlow/e4b68079-faad-4dde-ad77-771d7b962bb5.txt
void dfs(vector<vector<int>> &g,int k,int level,vector<int> &fa,vector<int> &id,int lenFa,int var_num,int flow_sum,vector<int> &flow){
    for(int i=1;i<=k;i++){
        int flag=rand()%2;
        int x=fa[i];
        if(flag) x=-x;
        if(i==1) {flow[x+var_num]=flow_sum;continue;}
        int bili=rand()%10+1;
        int one=flow_sum/10;
        flow[x+var_num]=one*bili;
        flow[-x+var_num]=flow_sum-one*bili;
    }
    flow_sum=flow_sum/10;
    for(int i=k+1;i<=var_num/2;i++){
        int flag=rand()%2;
        int x=fa[i];
        if(flag) x=-x;
        int bili=rand()%10+1;
        int one=flow_sum/10;
        flow[x+var_num]=one*bili;
        flow[-x+var_num]=flow_sum-one*bili;
    }
    flow_sum/=10;
    for(int i=var_num/2+1;i<=var_num;i++){
        int flag=rand()%2;
        int x=fa[i];
        if(flag) x=-x;
        int sum=rand()%flow_sum+1;
        int bili=rand()%10+1;
        int one=sum/10;
        
        flow[x+var_num]=one*bili;
        flow[-x+var_num]=sum-one*bili;
    }
  
}
void creatVarHigh(int var_num,ostream& outputStream){
    vector<vector<int>> g(2*var_num+1);
    vector<pair<int,int>> pr; 
    vector<int> flow((var_num)*2+1,0);
    int k=log(2*var_num);k=(k==0?1:k);
    for(int i=0;i<var_num;i++){
        pr.push_back(make_pair(rand()%10000, i+1));
        pr.push_back(make_pair(rand()%10000, -i-1));
    }
    sort(pr.begin(),pr.end());
    for(int i=0;i<k;i++){
        int liter=pr[i].second;
        flow[liter+var_num]=rand()%1000+1000;
    }
    for(int i=0;i<flow.size();i++){
        outputStream<<flow[i]<<" ";
    }
    outputStream<<endl;
   
    
}
void normal(int var_num,ostream& outputStream){
    
}
void creatVarTree(int var_num,ostream& outputStream){
    vector<vector<int>> g(var_num+1);
    vector<pair<int,int>> pr(var_num); 
    int k=log(2*var_num);k=(k==0?1:k);
    for(int i=0;i<var_num;i++){
        pr[i].first=rand()%100;
        pr[i].second=i+1;
    }
    sort(pr.begin(),pr.end());
    reverse(pr.begin(),pr.end());
    int root=pr.front().second;
    vector<int> fa(var_num+1,0), id(var_num+1,0);
    for(int i=0;i<var_num;i++){
        int u=pr[i].second;
        int pos=i+1;
        fa[pos]=u;
        id[u]=pos;
    }
    vector<int> flow((var_num)*2+1,0);
    int flow_num=2000;
    dfs(g,k,1,fa,id,1,var_num,flow_num,flow);
    for(int i=0;i<flow.size();i++){
        outputStream<<flow[i]<<" ";
    }
    outputStream<<endl;

    
}
///home/hyw/workspace/DatabaseAttackAndVerification/tu-rbowrapper-master/build/sat_test --creatFlow /home/hyw/workspace/DatabaseAttackAndVerification/specification_data/bd317aa5-b025-411f-b3bf-911b5ea83270/e4b68079-faad-4dde-ad77-771d7b962bb5.cnf /home/hyw/workspace/DatabaseAttackAndVerification/tu-rbowrapper-master/vitrualFlow/e4b68079-faad-4dde-ad77-771d7b962bb5.txt > /home/hyw/workspace/DatabaseAttackAndVerification/tu-rbowrapper-master/log/e4b68079-faad-4dde-ad77-771d7b962bb5.txt
void creatRandomFlow(int var_num,ostream& outputStream){
    vector<int> sceneCoverage;
    sceneCoverage.resize(var_num*2+1,0);
    for(auto &i:sceneCoverage){
        int t=rand()%100+1;
        if(t>90) i=rand()%100;
        else i=rand()%10;
    }
    for(auto i:sceneCoverage) outputStream<<i<<" ";outputStream<<endl;
    
}
void creatFlow(const string &Fcnf,const string &Foutput){
    srand(time(0));
    //INPUT
    ifstream fcnf(Fcnf, ios::in);
    int var_num,clause_num;
    string tmp;
    fcnf>>tmp>>tmp>>var_num>>clause_num;
    fcnf.close();
    //creat_flow
    ofstream fout(Foutput, ios::out);
    // if(var_num<=3)
        // creatRandomFlow(var_num,fout);
    // else
        creatVarHigh(var_num,fout);
    fout.close();
    
}
// ./build/sat
