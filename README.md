# 2QBF

## Compile
g++ -std=c++11 ./2QBF.cpp -o 2QBF

## Run
./2QBF < input.cnf

## Input Format
[clause number] [literal number]
[univeral or existential]  (1 1 1 0 0 0 means l1,l2,l3 is universal literal.l4,l5,l6 is existential literal)
[c1]   (c1 = 1 2 -3 -4 -5 means c1 is composed by: l1 or l2 or notl3 or notl4 or not5)
.
.
[cn]

## File Description
solver.h : SAT solver used when solving 2QBF
2QBF.cpp : solving 2QBF
input.cnf :

## Reference
[SAT solver](https://github.com/sukrutrao/SAT-Solver-CDCL)
