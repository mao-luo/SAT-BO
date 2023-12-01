import os
import re
import glob
import random
import csv
import time

# 运行时间限制
standTime=7200
# 运行次数
times=1
# 获取当前工作目录
work_Pth=os.getcwd()
print(work_Pth)
# 工作目录名称
researchPath='//Users//hyw//Desktop'
researchTopic='DATA'
# ----------------------------------------------
dataFile="specification_data//1ee0e949-d186-4b94-a0fd-efc55d06a7e2"
ctrlstr="walksat"
ctrlstr="check"
ctrlstr="vitrual"
ansName="ans.csv"
# ----------------------------------------------
runfilename=['sat_test']
condition=["efe4dd63-ed0b-4426-8a19-ead77fe3f78b.cnf"]
# condition=["e*"]

def getFile(path,condition):
    g = os.walk(path)  

    pathfile=[]
    for path,dir_list,file_list in g:  
        for condi in condition:
            for file_name in glob.glob(os.path.join(path,condi)):  
                pathfile.append(file_name)
    # print(pathfile)
    return pathfile
def getOutName(inputName):
    return inputName.split('.')[0]+".sol"
def getVarName(inputName):
    return inputName.split('.')[0]+".vw"
def getFileName(inputName):
    return inputName.split('/')[-1]

def write(ansPath,runFiles,files,tim,rand,r,target,m,n,nodeNum,cro):
    with open(ansPath, 'w') as f:
        write = csv.writer(f, dialect=('excel'))
        write.writerow(["runfile","instance","m","n","time","seed","m/n","target","nodeNum","cro"])
        k=0
        for i in range(len(files)):
            write.writerow([runFiles[i].split('/')[-1],files[i],m[i],n[i], tim[i],rand[i],r[i],target[i],nodeNum[i],cro[i]])
            
def running(makePth,inPath,outPath,varPath,runFiles,ansPath,errPath,condition):
    # print(make)
    os.system(makePth)
    # print(mak)
    files=getFile(inPath,condition)
    for file in files:
        fileName=getFileName(file)
        # print(fileName)
        outFile=os.path.join(outPath,getOutName(fileName))
        varFile=os.path.join(varPath,getVarName(fileName))
        errFile=os.path.join(errPath,getOutName(fileName))
        for j in range(times):
            seed=random.randint(100,1000000)
            for runFile in runFiles:
                
                if(ctrlstr=="check"):
                    config="--"+ctrlstr+" "+file+" "+outFile
                elif(ctrlstr=="walksat"):
                    config="--"+ctrlstr+" "+file+" "+outFile+" "+varFile
                else :
                    config="--"+ctrlstr+" "+outFile
                order=runFile+" "+config
                print(order)
                start = time.time()
                os.system(order)
                end = time.time()
                runTime = end - start

            
    # write(ansPath,runnames,filenames,tim,rand,r,target,m,n,nodeNums,cro)




research=os.path.join(researchPath,researchTopic)
buildPath=os.path.join(research,"build")
makePth='cd '+buildPath+' && make'
dataPath =os.path.join(research,dataFile)
outPath =os.path.join(research,"solve")
varPath=os.path.join(research,"var_weight")
errPath =os.path.join(research,"err")
runFile =[]
ansPath =os.path.join(research,ansName)
for name in runfilename:
    runFile.append(os.path.join(buildPath,name))
# files = getFile(inPath)
running(makePth,dataPath,outPath,varPath,runFile,ansPath,errPath,condition)