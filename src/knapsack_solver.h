#ifndef KNAPSACK_SOLVER_H
#define KNAPSACK_SOLVER_H

#include "ido_solver.h"

class KnapsackSolver : public IDOSolver {
public:
    KnapsackSolver(const char* filename);
    
private:
    void solveImpl() override;
};

#endif
