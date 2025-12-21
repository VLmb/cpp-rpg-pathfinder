#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "../core/model/Grid.h"
#include "../core/model/Hero.h"
#include "../core/path-find/AStarFinder.h"
#include "TestUtils.h"

static void fillGrid(Grid& grid, const CellType& cell) {
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            grid.setCellType(x, y, cell);
        }
    }
}

void testStraightLinePath() {
    std::cout << "\n--- Running testStraightLinePath ---\n";
    Grid grid(3, 3);
    CellType plain{1.0f, 1.0f, 1.0f};
    fillGrid(grid, plain);

    Human hero;
    AStarFinder astar(grid);

    std::vector<Point> expected{{0, 0}, {1, 0}, {2, 0}};
    auto path = astar.findPath({0, 0}, {2, 0}, hero);

    bool sameSize = path.size() == expected.size();
    bool sameContent = sameSize && std::equal(path.begin(), path.end(), expected.begin(),
        [](const Point& a, const Point& b) { return a == b; });

    assert_custom(sameSize && sameContent, "Straight path matches expected sequence");
}

void testAvoidsCostlyCell() {
    std::cout << "\n--- Running testAvoidsCostlyCell ---\n";
    Grid grid(3, 3);
    CellType plain{1.0f, 1.0f, 1.0f};
    CellType slow{0.01f, 0.01f, 0.01f};
    fillGrid(grid, plain);
    grid.setCellType(1, 1, slow);

    Human hero;
    AStarFinder astar(grid);

    auto path = astar.findPath({0, 1}, {2, 1}, hero);

    bool avoidsCenter = std::none_of(path.begin(), path.end(), [](const Point& p) {
        return p.x == 1 && p.y == 1;
    });
    bool lengthIsDetour = path.size() == 5;

    assert_custom(!path.empty(), "Path exists around obstacle");
    assert_custom(avoidsCenter, "Path does not use costly center cell");
    assert_custom(lengthIsDetour, "Path length reflects detour around center");
}

void testStartEqualsGoal() {
    std::cout << "\n--- Running testStartEqualsGoal ---\n";
    Grid grid(3, 3);
    CellType plain{1.0f, 1.0f, 1.0f};
    fillGrid(grid, plain);

    Human hero;
    AStarFinder astar(grid);

    Point start{1, 1};
    auto path = astar.findPath(start, start, hero);

    bool singleStep = path.size() == 1 && path.front() == start;
    assert_custom(singleStep, "Start equals goal returns single-node path");
}

void testInvalidPoints() {
    std::cout << "\n--- Running testInvalidPoints ---\n";
    Grid grid(3, 3);
    CellType plain{1.0f, 1.0f, 1.0f};
    fillGrid(grid, plain);

    Human hero;
    AStarFinder astar(grid);

    bool threw = false;
    try {
        astar.findPath({-1, 0}, {1, 1}, hero);
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    assert_custom(threw, "Invalid start triggers exception");
}

void runAStarTests() {
    std::cout << "\n=== Running A* Tests ===\n";
    testStraightLinePath();
    testAvoidsCostlyCell();
    testStartEqualsGoal();
    testInvalidPoints();
}
