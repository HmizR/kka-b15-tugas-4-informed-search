#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include "astar.hpp"
#include "gbfs.hpp"

namespace {
struct Measurement {
    SearchResult result;
    double averageTimeUs;
};

using Search = SearchResult (*)(const Grid&);

Measurement measure(Search algorithm, const Grid& grid, int repetitions) {
    // One untimed run supplies the path and metrics and warms up the algorithm.
    SearchResult result = algorithm(grid);
    double totalUs = 0.0;
    for (int i = 0; i < repetitions; ++i) {
        const auto start = std::chrono::steady_clock::now();
        SearchResult timedResult = algorithm(grid);
        const auto stop = std::chrono::steady_clock::now();
        totalUs += std::chrono::duration<double, std::micro>(stop - start).count();
        // Read the result outside the timed interval.
        if (timedResult.found != result.found || timedResult.pathCost != result.pathCost) {
            throw std::runtime_error("Algorithm returned inconsistent results.");
        }
    }
    return {result, totalUs / repetitions};
}

void report(std::ostream& csv, const std::string& name,
            const std::string& algorithm, const Measurement& measurement) {
    const auto& result = measurement.result;
    std::cout << std::left << std::setw(14) << algorithm
              << std::setw(9) << (result.found ? "true" : "false")
              << std::setw(12) << result.pathCost
              << std::setw(17) << result.expandedNodes
              << std::setw(19) << result.maxFrontierSize
              << measurement.averageTimeUs << '\n';
    csv << name << ',' << algorithm << ',' << (result.found ? "true" : "false")
        << ',' << result.pathCost << ',' << result.expandedNodes
        << ',' << result.maxFrontierSize << ',' << measurement.averageTimeUs << '\n';
}
} // namespace

int main(int argc, char* argv[]) {
    try {
        int repetitions = 1000;
        if (argc > 2) {
            throw std::runtime_error("Usage: comparison [positive repetition count]");
        }
        if (argc == 2) {
            std::size_t consumed = 0;
            const std::string argument = argv[1];
            repetitions = std::stoi(argument, &consumed);
            if (consumed != argument.size() || repetitions <= 0) {
                throw std::runtime_error("Repetition count must be a positive integer.");
            }
        }

        struct TestCase { const char* name; const char* filename; };
        const TestCase cases[] = {
            {"Simple", "testcases/simple.txt"},
            {"Moderate", "testcases/moderate.txt"},
            {"Misleading", "testcases/misleading.txt"},
            {"Dense", "testcases/dense.txt"},
            {"Large", "testcases/large.txt"}
        };
        // Load everything before opening the CSV to preserve previous results on load failure.
        Grid grids[5];
        for (int i = 0; i < 5; ++i) {
            if (!grids[i].loadFromFile(cases[i].filename)) {
                throw std::runtime_error(std::string("Cannot load ") + cases[i].filename);
            }
        }
        std::ofstream csv("results/comparison.csv");
        if (!csv) {
            throw std::runtime_error("Cannot open results/comparison.csv. Run from the project root with results/ present.");
        }
        csv << "Case,Algorithm,Found,PathCost,ExpandedNodes,MaxFrontierSize,AverageTimeUs\n";
        csv << std::fixed << std::setprecision(3);
        std::cout << std::fixed << std::setprecision(3)
                  << "Repetitions per algorithm: " << repetitions << '\n';
        for (int i = 0; i < 5; ++i) {
            std::cout << "\n" << cases[i].name << '\n'
                      << std::left << std::setw(14) << "Algorithm"
                      << std::setw(9) << "Found" << std::setw(12) << "Path Cost"
                      << std::setw(17) << "Expanded Nodes"
                      << std::setw(19) << "Max Frontier Size" << "Average Time (us)\n";
            const Measurement astarResult = measure(aStar, grids[i], repetitions);
            const Measurement gbfsResult = measure(gbfs, grids[i], repetitions);

            report(csv, cases[i].name, "A*", astarResult);
            report(csv, cases[i].name, "GBFS", gbfsResult);

            std::cout << "\nGrid awal:\n";
            grids[i].print();

            std::cout << "\nPath A*:\n";
            grids[i].printWithPath(astarResult.result.path);

            std::cout << "\nPath GBFS:\n";
            grids[i].printWithPath(gbfsResult.result.path);
        }
        csv.flush();
        if (!csv) throw std::runtime_error("Failed to write comparison CSV.");
        std::cout << "\nSaved results/comparison.csv\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
