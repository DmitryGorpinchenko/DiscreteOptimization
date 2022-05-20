#ifndef GC_SOLVER_H
#define GC_SOLVER_H

#include "ido_solver.h"

#include <vector>

class GCSolver : public IDOSolver {
public:
    GCSolver(const char* filename);
    
    void setColorsNumberHint(int colors_num);
    
private:
    void solveImpl() override;
    int calcObjectiveVal(const std::vector<int>& _x) const override;
    bool verifyConstraints(const std::vector<int>& _x) const override;

    const std::vector<std::vector<int>> g;
    int colors_num_hint;
};

#endif
