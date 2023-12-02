# SAT-BO
An Efficient SAT-Based Bayesian Optimization Algorithm for Verification Rules High-Risk Vulnerability Detection

#### Compiler Environmen

Running on linux platform

##### python

see ./requirements.txt

```
cd codes
python setup.py install
```

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
cd codes 
python main.py --input ../data/6e3abcf2-76fb-45df-adbf-607ce5cee26f.cnf --num 3 --traffic virtualFlow/case1/binomial/6e3abcf2-76fb-45df-adbf-607ce5cee26f.txt --runName sat_optimize
```

sat_optimize is our Adaptive DPLL SAT Solver in paper

#### Directory Overview:

```
./SAT //SAT solver
./turbo //Bayes algorithm
./var_weight //Variable attribute preference file
./solve //Solution file used to store SAT solver's output
./Ans：//It is the solution file of traffic coverage corresponding to the calculation returned by SAT.
./virtualFlow //Three generated traffic environment paths
```

