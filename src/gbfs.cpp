#include "gbfs.hpp"

#include <algorithm>
#include <cstddef>
#include <queue>
#include <vector>

#include "heuristic.hpp"

namespace {

struct Node {
    int h;                 
    std::size_t order;      
    Position pos;
};

struct NodeCompare {
    bool operator()(const Node& a, const Node& b) const {
        if (a.h != b.h) return a.h > b.h;
        return a.order > b.order;
    }
};

}

SearchResult gbfs(const Grid& grid) {
    SearchResult result;

    const Position start = grid.getStart();
    const Position goal = grid.getGoal();
    const int rows = grid.getRows();
    const int cols = grid.getCols();

    if (!grid.isValid(start) || !grid.isValid(goal) ||
        grid.isObstacle(start) || grid.isObstacle(goal)) {
        return result; 
    }

    const auto idx = [cols](Position p) {
        return static_cast<std::size_t>(p.row) * static_cast<std::size_t>(cols) +
               static_cast<std::size_t>(p.col);
    };
    const std::size_t total =
        static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols);

    std::vector<char> discovered(total, 0);
    std::vector<Position> parent(total, Position{-1, -1});
    std::vector<int> steps(total, 0);

    std::priority_queue<Node, std::vector<Node>, NodeCompare> frontier;
    std::size_t insertionCounter = 0;

    frontier.push(Node{manhattanDistance(start, goal), insertionCounter++, start});
    discovered[idx(start)] = 1;
    steps[idx(start)] = 0;
    result.maxFrontierSize = static_cast<int>(frontier.size());

    while (!frontier.empty()) {
        const Node current = frontier.top();
        frontier.pop();

        if (current.pos == goal) {
            result.found = true;
            result.pathCost = steps[idx(goal)];

            std::vector<Position> path;
            Position p = goal;
            while (p != start) {
                path.push_back(p);
                p = parent[idx(p)];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            result.path = std::move(path);
            return result;
        }

        ++result.expandedNodes;

        for (const Position& next : grid.getNeighbors(current.pos)) {
            if (discovered[idx(next)]) continue;

            discovered[idx(next)] = 1;
            parent[idx(next)] = current.pos;
            steps[idx(next)] = steps[idx(current.pos)] + 1;

            frontier.push(
                Node{manhattanDistance(next, goal), insertionCounter++, next});
            result.maxFrontierSize = std::max(
                result.maxFrontierSize, static_cast<int>(frontier.size()));
        }
    }

    result.found = false;
    result.path.clear();
    result.pathCost = -1;
    return result;
}
