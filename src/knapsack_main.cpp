#include "knapsack_solver.h"

#include <iostream>

int main(int argc, char *argv[]) {
    KnapsackSolver slv(argv[1]);
    slv.solve();
    slv.printSolution(std::cout);
    return 0;
}
