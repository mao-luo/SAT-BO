#include "vitrualEnvCaucute.h"

#include <fstream>
#include <iostream>
#include<ctime>
#include "ioBase.h"
#define ll long long
using namespace std;
ll ksm(ll a,ll b,ll mod){ll ans=1;while(b){if(b&1)(ans*=a)%=mod;(a*=a)%=mod;b>>=1;}return ans;}
int getXId(int x,int var_num){
    return x+var_num;
}
ll g(int x,int var_num,ll mod){
    //(x^{3}-1)/(x-1)
    ll ans=(ksm(x,3,mod)+mod-1)%mod*ksm((x+mod-1)%mod,mod-2,mod)%mod;
    return ans;
}
void sum(ll mod,Solutions &s,vector<ll> &sceneCoverage,vector<ll> &ansers){

    for(int i=0;i<s.solution_num;++i){
        vector<int> &sol=s.solutions[i];
        ll ans=0;
        for(auto it:sol){
            ans+=g(sceneCoverage[getXId(it,s.var_num)],s.var_num,mod);
            ans%=mod;
        }
        ansers.push_back(ans);
    }
}
void vitrual(const string &Fsolve,const string &FFlow,const string &Foutput){
    ll mod=1e9+7;
    srand(time(0));
    Solutions s;
    ReadFromFsolve(Fsolve,s);
    vector<ll> sceneCoverage;
    ifstream fin(FFlow, ios::in);
    int t;
    while(fin>>t){
        sceneCoverage.push_back(t);
    }
    fin.close();
    vector<ll> ansers;
    sum(mod,s,sceneCoverage,ansers);
    ofstream fout(Foutput, ios::out);
    for(auto i:ansers) fout<<i<<endl;
    fout.close();
    // for(auto i:sceneCoverage) cout<<i<<" ";cout<<endl;
    // cout<<sceneCoverage[getXId(-13,s.var_num)]<<" ";
    // cout<<sceneCoverage[getXId(13,s.var_num)];
}
