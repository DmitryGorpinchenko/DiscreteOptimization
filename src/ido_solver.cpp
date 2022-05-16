#include "ido_solver.h"

void IDOSolver::solve() {
    solveImpl();
    is_valid = verifySolution(obj_val, x);
}

bool IDOSolver::verifySolution(int _obj_val, const std::vector<int>& _x) const {
    return (_x.size() == n) && (_obj_val == calcObjectiveVal(_x)) && verifyConstraints(_x);
}

void IDOSolver::printSolution(std::ostream& os) {
    if (hasValidSolution()) {
        os << obj_val << " " << is_opt << "\n";

        for (size_t i = 0; i < x.size(); ++i) {
            os << x[i] << " ";
        }
        os << "\n";
    } else {
        os << "Solution is invalid!\n";
    }
}

void IDOSolver::addConstraint(SparseVector sv) {
    a.push_back(std::move(sv));
}

int IDOSolver::calcObjectiveVal(const std::vector<int>& _x) const {
    int res = 0;
    for (size_t i = 0; i < _x.size(); ++i) {
        res += c[i] * _x[i];
    }
    return res;
}

bool IDOSolver::verifyConstraints(const std::vector<int>& _x) const {
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].dot(_x) > b[i]) {
            return false;
        }
    }
    return true;
}
