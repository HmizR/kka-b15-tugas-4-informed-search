#pragma once

#include "grid.hpp"
#include "types.hpp"

// Greedy Best-First Search
// Priority: f(n) = h(n)
SearchResult gbfs(const Grid& grid);
