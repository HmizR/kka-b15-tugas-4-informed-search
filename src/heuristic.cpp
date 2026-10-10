#include "heuristic.hpp"

#include <cstdlib>

int manhattanDistance(Position a, Position b) {
    return std::abs(a.row - b.row) + std::abs(a.col - b.col);
}