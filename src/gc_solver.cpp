#include "gc_solver.h"
#include "bounded_set.h"

#include <fstream>
#include <algorithm>
#include <utility>

namespace {

std::vector<std::vector<int>> ReadGraph(const char* filename) {
    std::vector<std::vector<int>> res;
    
    std::ifstream is(filename);
    
    size_t n, e;
    is >> n >> e;
    
    res.assign(n, std::vector<int>());
    for (size_t i = 0; i < e; ++i) {
        int v1, v2;
        is >> v1 >> v2;
        res[v1].push_back(v2);
        res[v2].push_back(v1);
    }
    
    return res;
}

struct ConstraintStore {
    const std::vector<std::vector<int>>& g;
    std::vector<BoundedSet> domain_store;
    
    ConstraintStore(const std::vector<std::vector<int>>& _g)
        : g(_g)
    {
    }
    
    void reset(int max_colors_num) {
        domain_store.assign(g.size(), BoundedSet(max_colors_num, true));
    }
    
    bool isFeasible(int v, int c) const {
        if (!domain_store[v].contains(c)) {
            return false;
        }
        const auto& adj = g[v];
        for (size_t j = 0; j < adj.size(); ++j) {
            const auto& d = domain_store[adj[j]];
            if (d.contains(c) && (d.size() == 1)) {
                return false;
            }
        }
        return true;
    }
    
    void pruneDomains(int v, int c) {
        const auto& adj = g[v];
        for (size_t j = 0; j < adj.size(); ++j) {
            domain_store[adj[j]].erase(c);
        }
    }
    
    void restoreDomains(int v, int c, const std::vector<int>& colors) {
        const auto shouldBeInDomain = [this, &colors](int v, int c) {
            const auto& adj = g[v];
            for (size_t i = 0; i < adj.size(); ++i) {
                if (colors[adj[i]] == c) {
                    return false;
                }
            }
            return true;
        };
        const auto& adj = g[v];
        for (size_t j = 0; j < adj.size(); ++j) {
            if (const int w = adj[j]; shouldBeInDomain(w, c)) {
                domain_store[w].insert(c);
            }
        }
    }
};

struct SearchCtx {
    ConstraintStore cs;
    std::vector<int> colors;
    BoundedSet colored;
    int max_colors_num;
    int used_colors_num;
    bool is_opt;
    
    SearchCtx(const std::vector<std::vector<int>>& _g)
        : cs(_g)
        , colored(_g.size())
        , max_colors_num(_g.size())
        , used_colors_num(0)
        , is_opt(false)
    {
    }
    
    int selectNext() const {
        int res = -1;
        for (size_t i = 0; i < cs.g.size(); ++i) {
            if (!colored.contains(i)) {
                if ((res == -1) || (cs.domain_store[i].size() < cs.domain_store[res].size())) {
                    res = i;
                }
            }
        }
        return res;
    }
    
    bool runSearch(int _max_colors_num) {
        max_colors_num = (_max_colors_num > 0 && _max_colors_num <= int(cs.g.size())) ? _max_colors_num : int(cs.g.size());
        used_colors_num = 0;
        colors.assign(cs.g.size(), -1);
        colored.clear();
        cs.reset(max_colors_num);
        return runSearchImpl(selectNext());
    }
    
    bool runSearchImpl(int v) {
        for (int c = 0, count = std::min(max_colors_num, used_colors_num + 1); c < count; ++c) {
            if (tryColor(v, c)) {
                return true;
            }
        }
        return false;
    }
    
    bool tryColor(int v, int c) {
        if (cs.isFeasible(v, c)) {
            cs.pruneDomains(v, c);
            const auto cache = assignColor(v, c);
            if (colored.full() || runSearchImpl(selectNext())) {
                return true;
            }
            clearColor(v, cache);
            cs.restoreDomains(v, c, colors);
        }
        return false;
    }
    
    int assignColor(int v, int c) {
        colors[v] = c;
        colored.insert(v);
        const int prev_used_colors_num = used_colors_num;
        if (c + 1 > used_colors_num) {
            used_colors_num = c + 1;
        }
        return prev_used_colors_num;
    }
    
    void clearColor(int v, int prev_used_colors_num) {
        colors[v] = -1;
        colored.erase(v);
        used_colors_num = prev_used_colors_num;
    }
};

} // end namespace

GCSolver::GCSolver(const char* filename)
    : g(ReadGraph(filename))
    , colors_num_hint(g.size())
{
    n = g.size();
}

void GCSolver::setColorsNumberHint(int colors_num) {
    if (colors_num > 0 && colors_num <= int(n)) {
        colors_num_hint = colors_num;
    }
}

void GCSolver::solveImpl() {
    if (SearchCtx ctx(g); ctx.runSearch(colors_num_hint)) {
        obj_val = ctx.used_colors_num;
        is_opt = ctx.is_opt;
        x = std::move(ctx.colors);
    }
}

int GCSolver::calcObjectiveVal(const std::vector<int>& _x) const {
    return (*std::max_element(_x.begin(), _x.end())) + 1;
}

bool GCSolver::verifyConstraints(const std::vector<int>& _x) const {
    for (size_t i = 0; i < n; ++i) {
        const int c = _x[i];
        const auto& adj = g[i];
        for (size_t j = 0; j < adj.size(); ++j) {
            if (_x[adj[j]] == c) {
                return false;
            }
        }
    }
    return true;
}
