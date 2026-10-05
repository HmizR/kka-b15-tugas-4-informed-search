#pragma once

#include <string>
#include <vector>
#include "types.hpp"

class Grid {
private:
    std::vector<std::vector<int>> cells;
    Position start;
    Position goal;

public:
    // Load grid from a text file
    bool loadFromFile(const std::string& filename);

    // Check whether a position is inside the grid
    bool isValid(Position pos) const;

    // Check whether a position is an obstacle
    bool isObstacle(Position pos) const;

    // Get all walkable neighbors
    std::vector<Position> getNeighbors(Position pos) const;

    // Get start and goal positions
    Position getStart() const;
    Position getGoal() const;

    // Get grid dimensions
    int getRows() const;
    int getCols() const;

    // Display the grid
    void print() const;

    // Display the grid with a solution path
    void printWithPath(
        const std::vector<Position>& path
    ) const;
};
