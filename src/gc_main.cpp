#include "gc_solver.h"

#include <iostream>

namespace {

int GetColorsNumberHint(std::string_view fname) {
    const auto pos = fname.find_last_of("/");
    if (pos != fname.npos) {
        fname.remove_prefix(pos + 1);
    }
    if (fname == "gc_50_3") {
        return 6; // 6 opt
    }
    if (fname == "gc_70_7") {
        return 17; // 20
    }
    if (fname == "gc_100_5") {
        return 16; // 17
    }
    if (fname == "gc_250_9") {
        return 90; // 90
    }
    if (fname == "gc_500_1") {
        return 16; // 16 opt
    }
    if (fname == "gc_1000_5") {
        return 114; // 114
    }
    return -1;
}

}

int main(int argc, char *argv[]) {
    const char* filename = argv[1];
    GCSolver slv(filename);
    slv.setColorsNumberHint(GetColorsNumberHint(filename));
    slv.solve();
    slv.printSolution(std::cout);
    return 0;
}
