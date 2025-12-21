#pragma once

#include "model/Grid.h"
#include "model/Hero.h"
#include "PathFinderInterface.h"
#include "data-structure/PriorityQueue.h"
#include <vector>
#include <limits>
#include <cmath>
#include <stdexcept>
#include <algorithm>

struct PointWithPriority {
    Point point;
    double timeFromStart; 
    double priority; 
};

class AStar : public PathFinderInterface<Point> {
private:
    Grid& grid;

    double manhattan(Point a, Point b) const {
        return static_cast<double>(std::abs(a.x - b.x) + std::abs(a.y - b.y));
    }

    double heuristic(Point a, Point b, Hero& hero) const {
        return manhattan(a, b) * hero.getMinTimeToCross();
    }

    std::vector<Point> buildPath(int endIdx, const std::vector<int>& previous) const {
        std::vector<Point> path;
        for (int cur = endIdx; cur != -1; cur = previous[cur]) {
            path.push_back(grid.pointOfIndex(cur));
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

public:
    explicit AStar(Grid& grid) : grid(grid) {}

    std::vector<Point> findPath(const Point& start, const Point& goal, Hero& hero) override {
        if (!grid.inBounds(start) || !grid.inBounds(goal)) {
            throw std::invalid_argument("Start or goal point is out of bounds");
        }

        const int totalNodes = grid.getWidth() * grid.getHeight();
        const double INF = std::numeric_limits<double>::max();

        std::vector<int> previous(totalNodes, -1);
        std::vector<double> bestTimeFromStart(totalNodes, INF);

        const int startIdx = grid.indexOf(start);
        const int goalIdx  = grid.indexOf(goal);

        bestTimeFromStart[startIdx] = 0.0;

        PriorityQueue<PointWithPriority> pq([](const PointWithPriority& a, const PointWithPriority& b) -> bool {
            return a.priority < b.priority;
        });

        pq.enqueue(PointWithPriority{ start, 0.0, heuristic(start, goal, hero) });

        const Point directions[4] = {{1,0}, {-1,0}, {0,1}, {0,-1}};

        while (!pq.isEmpty()) {
            PointWithPriority cur = pq.extractMin();
            const int currentIdx = grid.indexOf(cur.point);

            if (cur.timeFromStart >= bestTimeFromStart[currentIdx]) {
                continue;
            }

            if (currentIdx == goalIdx) {
                return buildPath(goalIdx, previous);
            }

            for (const auto& dir : directions) {
                Point neighbor{ cur.point.x + dir.x, cur.point.y + dir.y };
                if (!grid.inBounds(neighbor)) continue;

                int neighborIdx = grid.indexOf(neighbor);

                double stepCost = hero.getTimeToCross(grid.getCellType(neighbor.x, neighbor.y));

                double newG = bestTimeFromStart[currentIdx] + stepCost;

                if (newG < bestTimeFromStart[neighborIdx]) {
                    bestTimeFromStart[neighborIdx] = newG;
                    previous[neighborIdx] = currentIdx;

                    double newF = newG + heuristic(neighbor, goal, hero);
                    pq.enqueue(PointWithPriority{ neighbor, newG, newF });
                }
            }
        }

        return {};
    }
};
