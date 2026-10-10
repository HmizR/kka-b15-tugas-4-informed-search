
#include "astar.hpp"
#include "heuristic.hpp"

#include <algorithm>
#include <queue>
#include <vector>
#include <cstddef>

namespace {

bool samePosition(const Position& a, const Position& b) {
    return a.row == b.row && a.col == b.col;
}

std::vector<Position> reconstructPath(
    Position start,
    Position goal,
    const std::vector<std::vector<Position>>& parent,
    const std::vector<std::vector<bool>>& hasParent
) {
    std::vector<Position> path;
    Position current = goal;

    path.push_back(current);

    while (!samePosition(current, start)) {
        if (!hasParent[current.row][current.col]) {
            return {};
        }

        current = parent[current.row][current.col];
        path.push_back(current);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

} // namespace

SearchResult aStar(const Grid& grid) {
    SearchResult result{};

    result.found = false;
    result.pathCost = -1;
    result.expandedNodes = 0;
    result.maxFrontierSize = 0;

    Position start = grid.getStart();
    Position goal = grid.getGoal();

    int rows = grid.getRows();
    int cols = grid.getCols();

    using Entry = std::tuple<int, std::size_t, Position>;

    struct Compare {
        bool operator()(const Entry& a, const Entry& b) const {
            if (std::get<0>(a) != std::get<0>(b)) {
                return std::get<0>(a) > std::get<0>(b);
            }

            return std::get<1>(a) > std::get<1>(b);
        }
    };

    std::priority_queue<
        Entry,
        std::vector<Entry>,
        Compare
    > frontier;

    const int INF = 1000000000;

    std::vector<std::vector<int>> gScore(
        rows, std::vector<int>(cols, INF)
    );

    std::vector<std::vector<Position>> parent(
        rows, std::vector<Position>(cols, {-1, -1})
    );

    std::vector<std::vector<bool>> hasParent(
        rows, std::vector<bool>(cols, false)
    );

    std::size_t order = 0;

    gScore[start.row][start.col] = 0;

    frontier.push({
        manhattanDistance(start, goal),
        order++,
        start
    });

    result.maxFrontierSize = frontier.size();

    while (!frontier.empty()) {
        Entry entry = frontier.top();
        frontier.pop();

        int currentF = std::get<0>(entry);
        Position current = std::get<2>(entry);

        int currentG = gScore[current.row][current.col];

        if (currentF != currentG +
            manhattanDistance(current, goal)) {
            continue;
        }

        if (samePosition(current, goal)) {
            result.path = reconstructPath(
                start, goal, parent, hasParent
            );

            if (!result.path.empty()) {
                result.found = true;
                result.pathCost = currentG;
            }

            return result;
        }

        ++result.expandedNodes;

        for (Position neighbor : grid.getNeighbors(current)) {
            int newG = currentG + 1;

            if (newG < gScore[neighbor.row][neighbor.col]) {
                gScore[neighbor.row][neighbor.col] = newG;

                parent[neighbor.row][neighbor.col] = current;
                hasParent[neighbor.row][neighbor.col] = true;

                int h = manhattanDistance(neighbor, goal);
                int f = newG + h;

                frontier.push({
                    f,
                    order++,
                    neighbor
                });

                result.maxFrontierSize = std::max(
                    result.maxFrontierSize,
                    frontier.size()
                );
            }
        }
    }

    result.found = false;
    result.path.clear();
    result.pathCost = -1;

    return result;
}
