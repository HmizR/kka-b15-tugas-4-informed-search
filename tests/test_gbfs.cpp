#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "gbfs.hpp"
#include "grid.hpp"
#include "heuristic.hpp"
#include "types.hpp"

namespace {

Grid loadMap(const std::string& name) {
    Grid grid;
    const bool ok = grid.loadFromFile("testcases/" + name);
    if (!ok) {
        std::cerr << "Failed to load testcases/" << name << "\n";
        std::abort();
    }
    return grid;
}

void checkValidSolution(const Grid& grid, const SearchResult& r) {
    assert(r.found);
    assert(!r.path.empty());

    assert(r.path.front() == grid.getStart());
    assert(r.path.back() == grid.getGoal());

    for (const Position& p : r.path) {
        assert(grid.isValid(p));
        assert(!grid.isObstacle(p));
    }

    for (std::size_t i = 1; i < r.path.size(); ++i) {
        assert(manhattanDistance(r.path[i - 1], r.path[i]) == 1);
        bool isNeighbor = false;
        for (const Position& n : grid.getNeighbors(r.path[i - 1])) {
            if (n == r.path[i]) { isNeighbor = true; break; }
        }
        assert(isNeighbor);
    }

    assert(r.pathCost == static_cast<int>(r.path.size()) - 1);
    assert(r.expandedNodes >= 0);
    assert(r.maxFrontierSize >= 1);
}

void testSolvable(const std::string& file) {
    Grid g = loadMap(file);
    SearchResult r = gbfs(g);
    checkValidSolution(g, r);
    std::cout << "[PASS] " << file << "  cost=" << r.pathCost
              << " expanded=" << r.expandedNodes
              << " maxFrontier=" << r.maxFrontierSize << "\n";
}

void testNoSolution() {
    Grid g = loadMap("nosolution.txt");
    SearchResult r = gbfs(g);
    assert(!r.found);
    assert(r.path.empty());
    assert(r.pathCost == -1);
    assert(r.expandedNodes > 0);
    assert(r.maxFrontierSize >= 1);
    std::cout << "[PASS] nosolution.txt  expanded=" << r.expandedNodes
              << " maxFrontier=" << r.maxFrontierSize << "\n";
}

}  

int main() {
    testSolvable("simple.txt");
    testSolvable("moderate.txt");
    testSolvable("dense.txt");
    testSolvable("large.txt");
    testSolvable("misleading.txt");
    testNoSolution();
    std::cout << "All GBFS tests passed.\n";
    return 0;
}