#include "grid.hpp"

#include <fstream>
#include <iostream>
#include <set>

bool Grid::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;

    cells.clear();
    bool hasStart = false, hasGoal = false;
    std::string line;
    int r = 0;

    while (std::getline(in, line)) {
        while (!line.empty() && (line.back() == '\r' || line.back() == ' '))
            line.pop_back();
        if (line.empty()) continue;

        if (!cells.empty() && line.size() != cells[0].size()) return false;

        std::vector<int> row(line.size(), 0);
        for (int c = 0; c < static_cast<int>(line.size()); ++c) {
            switch (line[c]) {
                case '.': break;
                case '#': row[c] = 1; break;
                case 'S':
                    if (hasStart) return false;
                    start = Position{r, c};
                    hasStart = true;
                    break;
                case 'G':
                    if (hasGoal) return false;
                    goal = Position{r, c};
                    hasGoal = true;
                    break;
                default: return false;
            }
        }
        cells.push_back(std::move(row));
        ++r;
    }
    return !cells.empty() && hasStart && hasGoal;
}

bool Grid::isValid(Position pos) const {
    return pos.row >= 0 && pos.row < getRows() &&
           pos.col >= 0 && pos.col < getCols();
}

bool Grid::isObstacle(Position pos) const {
    return !isValid(pos) || cells[pos.row][pos.col] == 1;
}

std::vector<Position> Grid::getNeighbors(Position pos) const {
    static const int dr[4] = {-1, 0, 1, 0};
    static const int dc[4] = {0, 1, 0, -1};
    std::vector<Position> result;
    for (int i = 0; i < 4; ++i) {
        Position n{pos.row + dr[i], pos.col + dc[i]};
        if (isValid(n) && !isObstacle(n)) result.push_back(n);
    }
    return result;
}

Position Grid::getStart() const { return start; }
Position Grid::getGoal() const { return goal; }
int Grid::getRows() const { return static_cast<int>(cells.size()); }
int Grid::getCols() const { return cells.empty() ? 0 : static_cast<int>(cells[0].size()); }

void Grid::print() const { printWithPath({}); }

void Grid::printWithPath(const std::vector<Position>& path) const {
    std::set<std::pair<int, int>> onPath;
    for (const Position& p : path) onPath.insert({p.row, p.col});

    for (int r = 0; r < getRows(); ++r) {
        for (int c = 0; c < getCols(); ++c) {
            Position p{r, c};
            char ch = '.';
            if (cells[r][c] == 1) ch = '#';
            else if (p == start) ch = 'S';
            else if (p == goal) ch = 'G';
            else if (onPath.count({r, c})) ch = '*';
            std::cout << ch;
        }
        std::cout << '\n';
    }
}