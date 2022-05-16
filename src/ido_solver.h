#ifndef IDO_SOLVER_H
#define IDO_SOLVER_H

#include "sparse_vector.h"

#include <vector>
#include <ostream>

/*
 * DOSolver stands for Discrete Optimization Solver
 */
class IDOSolver {
public:
    virtual ~IDOSolver() = default;

    bool hasValidSolution() const { return is_valid; }
    bool isOptimal() const { return is_opt; }
    int getObjectiveVal() const { return obj_val; }
    const std::vector<int>& getX() const { return x; }
    
    void solve();
    bool verifySolution(int _obj_val, const std::vector<int>& _x) const;
    void printSolution(std::ostream& os);
    
protected:
    void addConstraint(SparseVector sv);

    size_t n = 0;
    std::vector<int> x;
    std::vector<int> c;
    std::vector<SparseVector> a;
    std::vector<int> b;
    int obj_val = 0;
    bool is_opt = false;
    
private:
    virtual void solveImpl() = 0;
    virtual int calcObjectiveVal(const std::vector<int>& _x) const;
    virtual bool verifyConstraints(const std::vector<int>& _x) const;
    
    bool is_valid = false;
};

#endif
