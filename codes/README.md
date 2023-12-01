# SAT-BO
An Efficient SAT-Based Bayesian Optimization Algorithm for Verification Rules High-Risk Vulnerability Detection

#### Compiler Environmen

##### python

see ./requirements.txt

##### c++

./SAT/CMakeLists.txt

```
cmake 3.22	
```

###### build

```
cd codes
mkdir build
cd build
cmake ../SAT/
make
```



#### Operating parameters

##### parameters setting

```
--input file //Input CNF Instance
--num //Number of SAT and Bayes iterations
--traffic //Simulation environment path
--runName //SAT Solver Path
```

##### Run  example

``` 
python main.py ../specification_data/0a764ddc-378b-49aa-a4a4-17de37715e86.cnf --num 3 --vitrual ./vitrualFlow/case1/0a764ddc-378b-49aa-a4a4-17de37715e86.txt --runName ./build/sat_optimize
```

sat_optimize is our Adaptive DPLL SAT Solver in paper

#### Directory Overview:

```
./SAT //SAT solver
./turbo //Bayes algorithm
./var_weitht //variable attribute preference file
./solve //Solution file used to store SAT solver output
./Ans：//It is the solution traffic coverage file corresponding to the calculation returned by SAT.
./virtualFlow //three generated traffic environment paths
```

