#pragma once

#include "grid.hpp"
#include "types.hpp"

// A* Search
// Priority: f(n) = g(n) + h(n)
SearchResult aStar(const Grid& grid);
