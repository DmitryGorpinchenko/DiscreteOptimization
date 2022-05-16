#include "knapsack_solver.h"

#include <fstream>

KnapsackSolver::KnapsackSolver(const char* filename) {
    std::ifstream is(filename);

    int k;
    is >> n >> k;
    b.push_back(k);
    
    c.resize(n);
    SparseVector w(n);
    for (size_t i = 0; i < n; ++i) {
        int vi, wi;
        is >> vi >> wi;
        
        c[i] = vi;
        w[i] = wi;
    }
    addConstraint(std::move(w));
}

void KnapsackSolver::solveImpl() {
    auto k = size_t(b.back());

    const auto& v = c;
    const auto w = a.back().toStdVector();

    std::vector<std::vector<bool>> taken_cache(n + 1, std::vector<bool>(k + 1, false));
    
    std::vector<int> prev(k + 1, 0);
    std::vector<int> curr(k + 1, 0);
    
    for (size_t i = 1; i <= n; ++i) {
        const int vi = v[i - 1];
        const int wi = w[i - 1];
        for (size_t j = 0; j <= k; ++j) {
            curr[j] = prev[j];
            if (wi <= j) {
                if (const int val = vi + prev[j - wi]; val > curr[j]) {
                    curr[j] = val;
                    taken_cache[i][j] = true;
                }
            }
        }
        curr.swap(prev);
    }
    
    obj_val = prev[k];
    is_opt = true;
    
    // trace back
    x.assign(n, 0);
    for (size_t i = n; i > 0; --i) {
        if (taken_cache[i][k]) {
            x[i - 1] = 1;
            k -= size_t(w[i - 1]);
        }
    }
}
