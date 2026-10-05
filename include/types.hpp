#pragma once

#include <vector>

struct Position {
    int row;
    int col;

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

struct SearchResult {
    bool found = false;
    std::vector<Position> path;
    int pathCost = -1;
    int expandedNodes = 0;
    int maxFrontierSize = 0;
};
