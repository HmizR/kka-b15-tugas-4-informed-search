
#include "astar.hpp"
#include "heuristic.hpp"

#include <algorithm>
#include <queue>
#include <vector>
#include <cstddef>
#include <tuple>

using namespace std;

namespace {

bool samePosition(Position a, Position b) {
    return a.row == b.row && a.col == b.col;
}

vector<Position> reconstructPath(
    Position start,
    Position goal,
    const vector<vector<Position>>& parent,
    const vector<vector<bool>>& hasParent
) {
    vector<Position> path;
    Position current = goal;

    while (!samePosition(current, start)) {
        path.push_back(current);

        if (!hasParent[current.row][current.col]) {
            return {};
        }

        current = parent[current.row][current.col];
    }

    path.push_back(start);
    reverse(path.begin(), path.end());

    return path;
}

struct Compare {
    bool operator()(
        const tuple<int, size_t, Position>& a,
        const tuple<int, size_t, Position>& b
    ) const {
        if (get<0>(a) != get<0>(b)) {
            return get<0>(a) > get<0>(b);
        }

        return get<1>(a) > get<1>(b);
    }
};

} 

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

    const int INF = 1000000000;

    vector<vector<int>> gScore(
        rows, vector<int>(cols, INF)
    );

    vector<vector<Position>> parent(
        rows, vector<Position>(cols, start)
    );

    vector<vector<bool>> hasParent(
        rows, vector<bool>(cols, false)
    );

    priority_queue<
        tuple<int, size_t, Position>,
        vector<tuple<int, size_t, Position>>,
        Compare
    > frontier;

    size_t order = 0;

    gScore[start.row][start.col] = 0;

    frontier.push({
        manhattanDistance(start, goal),
        order++,
        start
    });

    result.maxFrontierSize = frontier.size();

    while (!frontier.empty()) {
        auto currentEntry = frontier.top();
        frontier.pop();

        int currentF = get<0>(currentEntry);
        Position current = get<2>(currentEntry);

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

        result.expandedNodes++;

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
                    static_cast<int>(frontier.size())
                );
            }
        }
    }

    result.found = false;
    result.path.clear();
    result.pathCost = -1;

    return result;
}
