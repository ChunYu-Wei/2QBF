# 2QBF

## Compile
g++ -std=c++11 ./2QBF.cpp -o 2QBF

## Run
./2QBF < input.cnf

## File Description
solver.h : DPLL SAT solver used when solving 2QBF <br></br>
2QBF.cpp : solving 2QBF <br></br>
input.cnf : input file 

## Input Format
[clause number] [literal number] <br></br>
[univeral or existential]  (1 1 1 0 0 0 means l1,l2,l3 is universal literal.l4,l5,l6 is existential literal) <br></br>
[c1]   (c1 = 1 2 -3 -4 -5 means c1 is composed by: l1 or l2 or notl3 or notl4 or not5) <br></br>
. <br></br>
. <br></br>
[cn] 

## Reference
[SAT solver](https://github.com/sukrutrao/SAT-Solver-CDCL) <br></br>
[Janota1 and Silva,2015] Mikol´aˇs Janota1 and Joao Marques-Silva. Solving QBF by Clause Selection. In IJCAI 2015.
