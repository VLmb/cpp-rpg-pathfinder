#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "../core/path-find/MapPathfinder.h"
#include "TestUtils.h"

static void fillGrid(Grid& grid, const CellProperty& cell) {
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            grid.setCellType(x, y, cell);
        }
    }
}

void testMapStraightPath() {
    std::cout << "\n--- Running testMapStraightPath ---\n";
    Grid grid(3, 3);
    CellProperty plain{1.0f, 1.0f, 1.0f};
    fillGrid(grid, plain);

    Human hero;
    MapPathfinder pathfinder(&grid);

    auto result = pathfinder.findPath({0, 0}, {2, 0}, hero);

    std::vector<Point> expected{{0, 0}, {1, 0}, {2, 0}};
    bool pathMatches = result.path.size() == expected.size()
        && std::equal(result.path.begin(), result.path.end(), expected.begin(),
                      [](const Point& a, const Point& b) { return a == b; });
    bool timeMatches = std::abs(result.time - 2.0) < 1e-9;

    assert_custom(pathMatches, "Straight path matches expected sequence");
    assert_custom(timeMatches, "Straight path time equals 2 steps");
}

void testMapAvoidsCostlyCell() {
    std::cout << "\n--- Running testMapAvoidsCostlyCell ---\n";
    Grid grid(3, 3);
    CellProperty plain{1.0f, 1.0f, 1.0f};
    CellProperty slow{0.001f, 0.001f, 0.001f}; // very slow cell
    fillGrid(grid, plain);
    grid.setCellType(1, 1, slow);

    Human hero;
    MapPathfinder pathfinder(&grid);

    auto result = pathfinder.findPath({0, 1}, {2, 1}, hero);

    bool avoidsCenter = std::none_of(result.path.begin(), result.path.end(), [](const Point& p) {
        return p.x == 1 && p.y == 1;
    });
    bool detourLength = result.path.size() == 5; // expects detour around center
    bool detourTime = std::abs(result.time - 4.0) < 1e-6;

    assert_custom(!result.path.empty(), "Path exists around slow cell");
    assert_custom(avoidsCenter, "Path avoids costly center cell");
    assert_custom(detourLength, "Path length reflects detour");
    assert_custom(detourTime, "Detour time equals 4 steps");
}

void testMapStartEqualsGoal() {
    std::cout << "\n--- Running testMapStartEqualsGoal ---\n";
    Grid grid(3, 3);
    CellProperty plain{1.0f, 1.0f, 1.0f};
    fillGrid(grid, plain);

    Human hero;
    MapPathfinder pathfinder(&grid);

    Point start{1, 1};
    auto result = pathfinder.findPath(start, start, hero);

    bool singleNode = result.path.size() == 1 && result.path.front() == start;
    bool zeroTime = std::abs(result.time) < 1e-9;

    assert_custom(singleNode, "Start equals goal returns single node");
    assert_custom(zeroTime, "Start equals goal has zero time");
}

void testMapInvalidPoints() {
    std::cout << "\n--- Running testMapInvalidPoints ---\n";
    Grid grid(3, 3);
    CellProperty plain{1.0f, 1.0f, 1.0f};
    fillGrid(grid, plain);

    Human hero;
    MapPathfinder pathfinder(&grid);

    bool threw = false;
    try {
        pathfinder.findPath({-1, 0}, {1, 1}, hero);
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    assert_custom(threw, "Invalid start triggers exception");
}

void runMapPathfinderTests() {
    std::cout << "\n=== Running MapPathfinder Tests ===\n";
    testMapStraightPath();
    testMapAvoidsCostlyCell();
    testMapStartEqualsGoal();
    testMapInvalidPoints();
}
