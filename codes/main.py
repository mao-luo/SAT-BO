import random
from turbo import Turbo1
import numpy as np
import os
import torch
import math
import argparse


class Levy:
    def __init__(self, dim=10):
        self.dim = dim
        # 
        self.lb =   np.zeros(dim)
        self.ub =   np.ones(dim)

    def __call__(self, x):
        assert len(x) == self.dim
        assert x.ndim == 1
        assert np.all(x <= self.ub) and np.all(x >= self.lb)
        w = 1 + (x - 1.0) / 4.0
        val = np.sin(np.pi * w[0]) ** 2 + \
              np.sum((w[1:self.dim - 1] - 1) ** 2 * (1 + 10 * np.sin(np.pi * w[1:self.dim - 1] + 1) ** 2)) + \
              (w[self.dim - 1] - 1) ** 2 * (1 + np.sin(2 * np.pi * w[self.dim - 1]) ** 2)
        return val

class SAT:
    def __init__(self,current_directory,inputPath,vitrualPath,rnname):
        self.varNum=0
        self.solNum=0
        self.clauseNum=0
        self.inputPath=inputPath
        self.InputFileName=self.getFileName(inputPath)
        self.satRunFilePath=os.path.join(current_directory,'build/sat_test')
        self.satRunFilePath1=os.path.join(current_directory,'build/'+rnname)
        print(self.satRunFilePath1)
        self.solvePath=os.path.join(current_directory,os.path.join('solve',self.getSolName(self.InputFileName)))
        self.varWeightPath=os.path.join(current_directory,os.path.join('var_weight',self.getVarName(self.InputFileName)))
        self.vitrualFlowPath=os.path.join(current_directory,os.path.join(vitrualPath,self.getFlowName(self.InputFileName)))
        self.vitrualAnsPath=os.path.join(current_directory,os.path.join('Ans',self.getFlowName(self.InputFileName)))
        self.logPath=os.path.join(current_directory,os.path.join('log',self.getFlowName(self.InputFileName)))
        self.getVarNumAndSolNum()
        self.getclauseNum()
    def getclauseNum(self):
        f=open(self.inputPath,"r")
        mess=f.readline().split("\n")[0]
        self.clauseNum=int(mess.split(" ")[3])
    def getFileName(self,filePath):
        return filePath.split("\\")[-1].split('/')[-1]
    def getSolName(self,filename):
        return filename.split('.')[0]+'.sol'
    def getVarName(self,filename):
        return filename.split('.')[0]+'.vw'
    def getFlowName(self,filename):
        return filename.split('.')[0]+'.txt'
    def getTXTName(self,filename):
        return filename.split('.')[0]+'.txt'
    def runSAT(self,ctrlstr):
        if(ctrlstr=="walksat"):
            config=self.walksat()
        elif(ctrlstr=="randSample"):
            config=self.randSample()
        elif (ctrlstr=="check"):
            config=self.check()
        elif (ctrlstr=="creatFlow"):
            config=self.creatFlow()
        elif (ctrlstr=="vitrual"):
            config=self.vitrual()
        
        if(ctrlstr=="walksat"or ctrlstr=="randSample"):
            config=config+" 2> "+self.logPath
            order=self.satRunFilePath1+" "+config
        else:
            config=config+" > "+self.logPath
            order=self.satRunFilePath+" "+config
        # print(order)
        # if((ctrlstr=="creatFlow")):
        #     print(order)
        os.system(order)
    def check(self):
        config="--check"+" "+self.inputPath+" "+self.solvePath
        return config
    def walksat(self):
        config=self.inputPath+" "+self.solvePath+" "+self.varWeightPath+" "+str(30)+" "+str(30)
        return config
    def randSample(self):
        config=self.inputPath+" "+self.solvePath+" "+" "+str(30)+" "+str(30)
        return config
    def creatFlow(self):
        config="--creatFlow"+" "+self.inputPath+" "+self.vitrualFlowPath
        return config
    def vitrual(self):
        config="--vitrual"+" "+self.solvePath+" "+self.vitrualFlowPath+" "+self.vitrualAnsPath
        return config
    def getVarNumAndSolNum(self):
        # os.system(self.satRunFilePath1+" "+self.inputPath+" "+self.solvePath+" "+str(30)+" "+str(100)+" 2> "+self.logPath)
        # print(self.satRunFilePath1+" "+self.inputPath+" "+self.solvePath+" "+str(30)+" "+str(100))
        self.runSAT("randSample")
        f = open(self.solvePath)
        line = f.readline()
        infor=line.split(' ')
        self.varNum=int(infor[1])
        self.solNum=int(infor[2])
        f.close()
    def getX(self):
        f = open(self.solvePath)
        X=[]
        line = f.readline()
        infor=line.split(' ')
        for i in range(self.solNum):
            line = f.readline().strip('\n')
            sol=[]
            solu=[]
            for j in line.split(' '):
                if(j==""or j=="0"):
                    continue
                else:
                    sol.append(int(j))
            for j in sol :
                if(j<0):
                    solu.append(0.0)
                else:
                    solu.append(1.0)
            X.append(solu)
        f.close()
        return np.array(X)
    def getFX(self):
        f = open(self.vitrualAnsPath)
        FX=[]
        for i in range(self.solNum):
            line = f.readline().strip('\n')
            FX.append([float(line)])
        f.close()
        return np.array(FX)
    def getFX1(self,solNums):
        f = open(self.vitrualAnsPath)
        FX=[]
        for i in range(solNums):
            line = f.readline().strip('\n')
            FX.append([float(line)])
        f.close()
        return np.array(FX)
    def updataVarWeight(self,Weight):
        # print(len(Weight[0]))
        f = open(self.varWeightPath,'w')
        f.write(str(len(Weight))+'\n')
        for idweight in Weight:
            for i in idweight:
                f.write(str(i)+' ')
            f.write('\n')
        f.close()
    def getCandicatesFX(self,X):
        f = open(self.solvePath,'w')
        f.write('s'+' '+str(self.varNum)+' '+str(len(X))+'\n')
        for solu in X:
            for i in range(self.varNum):
                if(solu[i]<0.5):
                    f.write(str(-i-1)+' ')
                else:
                    f.write(str(i+1)+' ')
            f.write('0\n')
        f.close()
        self.runSAT("vitrual")
        return self.getFX1(len(X))
    def getFlow(self):
        f = open(self.vitrualFlowPath,'r')
        Flow=[]
        line = f.readline().strip('\n').split(" ")
        for i in range(self.varNum*2+1):
            Flow.append(int(line[i]))
        f.close()
        return np.array(Flow)
    def checkSolve(self,X):
        f = open(self.solvePath,'w')
        f.write("s "+str(self.varNum)+" "+str(len(X))+"\n")
        for sol in X:
            for i in range(len(sol)):
                if(sol[i]==1):
                    f.write(str(i+1)+" ")
                else:
                    f.write(str(-i-1)+" ")
            f.write("0\n")
        f.close()
        self.runSAT("check")
    def readCheck(self):
        f = open(self.logPath,'r')
        mess=f.readline().split('\n')[0]
        if(mess=="Solve is right!"):
            return 1
        else :
            return 0
        print(mess)
        

def getNum(x):
    return (1+x+x*x)


# python -u "/Users/hyw/Desktop/tu-rbowrapper-master/main.py" -i /Users/hyw/Desktop/data/1.cnf
def runTurbo(turbo,sat):
    # sat.runSAT("creatFlow")
    """Run the full optimization process."""
    kkk=0
    # print("maxeval")
    # print(turbo.max_evals)
    # print("solnum")
    # print(sat.solNum)
    dic={}
    unUse=[]
    while turbo.n_evals < turbo.max_evals-turbo.batch_size:
        # Generate and evalute initial design points
        # 3. （外部平台）将X修正为X'，获得X'对应的目标函数值fX'
        sat.runSAT("randSample")
        sat.getVarNumAndSolNum()
        sat.runSAT("vitrual")
        X_init = sat.getX()
        # print(X_init)
        fX_init = sat.getFX()
        # print(len(fX_init))
        # kkk+=1
        # print(turbo.n_evals)
        turbo.initial_solutions_of_trust_region(X_init, fX_init)
        # print(turbo.n_evals) 
        # print(turbo.max_evals)
        # Thompson sample to get next suggestions
        # print("ok")
        while turbo.n_evals < turbo.max_evals-turbo.batch_size and turbo.is_trust_region_big_enough():
            # print("a")
            X_next = turbo.suggested_sampling()
            # print("b")
            AssignmentPreferences=[]
            for recommend in X_next:
                iter=[]
                for j in recommend:
                    if(j>0.5):
                        iter.append(1)
                    else:
                        iter.append(0)
                if dic.get(str(iter))==None:
                    dic[str(iter)]=1
                    AssignmentPreferences.append(recommend)
            
            AssignmentPreference=[]
            if(len(AssignmentPreferences)>2):
                a=random.randint(1,len(AssignmentPreferences)-1)
                b=random.randint(a+1,len(AssignmentPreferences))
                a=a-1
                b=b-1
                for id in range(len(AssignmentPreferences)):
                    if id==a or id==b:
                        AssignmentPreference.append(AssignmentPreferences[id])
            elif(len(AssignmentPreferences)>0):
                AssignmentPreference=AssignmentPreferences
            else:
                AssignmentPreference.append(X_next[random.randint(1,len(X_next))-1])
            # print("c")
            sat.updataVarWeight(AssignmentPreference)
            sat.runSAT("walksat")
            sat.getVarNumAndSolNum()
            sat.runSAT("vitrual")
            X_next = sat.getX()
            fX_next = sat.getFX()

            X_next = turbo.select_from_candidates(X_next)
            fX_next=sat.getCandicatesFX(X_next)
            # Evaluate batch
            # fX_next = np.array([[turbo.f(x)] for x in X_next])
            # 9. 更新置信区间turbo1.solutions_and_obj_after_sampling(X, fX)
            turbo.solutions_and_obj_after_sampling(X_next, fX_next)
    # print("jiaohu:"+str(kkk))

def OneFlowAns(f,turbo1,sat):
    runTurbo(turbo1,sat)
    print("FLOW:")
    Flow=sat.getFlow()
    # print(Flow)
    x_best, f_best = turbo1.best_sol_and_obj()
    print(x_best)
    print("Best value found:\n\tf(x) = %.3f\nObserved at:\n\tx = %s" % (f_best, np.around(x_best, 3)))
    MAX_Cover=0
    Flow_Len=len(Flow)
    VarNum=(int)((Flow_Len-1)/2)
    for i in range(VarNum):
        MAX_Cover=MAX_Cover+getNum(max(Flow[i+VarNum+1],Flow[VarNum-(i+1)]))
    print("UpperLimitCover:\n\t"+str(MAX_Cover))
    TrueCover=0
    X=[]
    for i in range(len(x_best)):
        if(x_best[i]>0.5):
            X.append(1)
            TrueCover=TrueCover+getNum(Flow[i+1+VarNum])
        else :
            X.append(0)
            TrueCover=TrueCover+getNum(Flow[-(i+1)+VarNum])
    
    print("Cover:\n\t"+str(TrueCover))
    
    sat.checkSolve(np.array([X]))
    # 输出文件名，场景覆盖度上限，场景覆盖度,是否是正确解
    return [MAX_Cover,TrueCover,sat.readCheck()]
   

    # f.write(x_best)
def optimize(file,current_directory,inputPath,sat,times):
    f=Levy(sat.varNum)
    print("times: "+str(times))
    turbo1 = Turbo1(
        f=f,  # Handle to objective function
        lb=f.lb,  # Numpy array specifying lower bounds
        ub=f.ub,  # Numpy array specifying upper bounds

        # 解的数量
        n_init=sat.solNum,  # Number of initial bounds from an Latin hypercube design
        max_evals=(times-1)*min(10,sat.solNum)+sat.solNum+1,  # Maximum number of evaluations
        batch_size=min(10,sat.solNum) , # How large batch size TuRBO uses
        verbose=True,  # Print information from each batch
        use_ard=True,  # Set to true if you want to use ARD for the GP kernel
        max_cholesky_size=2000,  # When we switch from Cholesky to Lanczos
        n_training_steps=50,  # Number of steps of ADAM to learn the hypers
        min_cuda=1024,  # Run on the CPU for small datasets
        device="cpu",  # "cpu" or "cuda"
        dtype="float64",  # float64 or float32
    )
    
    return OneFlowAns(file,turbo1,sat)
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='argparse testing')
    parser.add_argument('--input','-i',type=str, default ="bk",required=True,help="input file")
    parser.add_argument('--num','-n',type=str, default =3,help="input file")
    parser.add_argument('--traffic','-v',type=str, default ="vitrualFlow",help="input file")
    parser.add_argument('--runName','-r',type=str, default ="sat_optimize",help="input file")
    
    args = parser.parse_args()
    current_directory = os.path.dirname(os.path.abspath(__file__))   
    sat=SAT(current_directory,args.input,args.vitrual,args.runName)
    f = open("./anser/"+sat.getTXTName(sat.InputFileName),'w')
    ansers=[]
    mylist=[]
    for t in range(int(1)):
        oneAns=optimize(f,current_directory,args.input,sat,int(args.num))
        ansers.append(oneAns)
        mylist.append((oneAns[1]*1.0/oneAns[0],t))
    sorted(mylist,key=lambda x:x[0])
    mylist.reverse()
    MaxCovers=0
    TrueCovers=0
    TrueNums=0
    bestNum=int(int(1)/2)+1
    bestNum=min(bestNum,int(1))
    print(bestNum)
    for i in range(bestNum):
        # print(mylist[i][1])
        MaxCovers=MaxCovers+ansers[mylist[i][1]][0]
        TrueCovers=TrueCovers+ansers[mylist[i][1]][1]
        TrueNums=TrueNums+ansers[mylist[i][1]][2]
        # print(ansers[mylist[i][1]][1])
    print(mylist)
    print(ansers)
    f.write(sat.InputFileName+"\t"+str(sat.varNum)+"\t"+str(sat.clauseNum)+"\t"+str(MaxCovers)+"\t"+str(TrueCovers)+"\t"+str(TrueNums)+"\n")
    f.close()
   