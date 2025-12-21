#pragma once

#include "../model/Grid.h"
#include "../model/Hero.h"
#include "PathFinderInterface.h"
#include "../data-structure/PriorityQueue.h"
#include "../data-structure/HashMap.h"
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

struct PathKey {
    Point start;
    Point goal;
    Hero* hero;
};

struct PathKeyEq {
    bool operator()(const PathKey& a, const PathKey& b) const noexcept {
        return a.start.x == b.start.x && a.start.y == b.start.y &&
               a.goal.x  == b.goal.x  && a.goal.y  == b.goal.y  &&
               a.hero == b.hero;
    }
};

static inline void hashCombine(std::size_t& seed, std::size_t v) noexcept {
    seed ^= v + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

struct PathKeyHash {
    std::size_t operator()(const PathKey& k) const noexcept {
        std::size_t seed = 0;
        hashCombine(seed, std::hash<int>{}(k.start.x));
        hashCombine(seed, std::hash<int>{}(k.start.y));
        hashCombine(seed, std::hash<int>{}(k.goal.x));
        hashCombine(seed, std::hash<int>{}(k.goal.y));
        hashCombine(seed, std::hash<const Hero*>{}(k.hero));
        return seed;
    }
};

class AStar : public PathFinderInterface<Point> {
private:
    Grid& grid;
    HashMap<PathKey, std::vector<Point>, PathKeyHash, PathKeyEq> pathCache;

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

        if (auto* cached = pathCache.getPtr({ start, goal, &hero })) {
            return *cached;
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

            if (cur.timeFromStart > bestTimeFromStart[currentIdx]) {
                continue;
            }

            if (currentIdx == goalIdx) {
                auto path = buildPath(goalIdx, previous);
                pathCache.put({start, goal, &hero}, path);
                return buildPath(goalIdx, previous);
            }

            for (const auto& dir : directions) {
                Point neighbor{ cur.point.x + dir.x, cur.point.y + dir.y };
                if (!grid.inBounds(neighbor)) continue;

                int neighborIdx = grid.indexOf(neighbor);

                double stepTime = hero.getTimeToCross(grid.getCellType(neighbor.x, neighbor.y));

                double newBestTime = bestTimeFromStart[currentIdx] + stepTime;

                if (newBestTime < bestTimeFromStart[neighborIdx]) {
                    bestTimeFromStart[neighborIdx] = newBestTime;
                    previous[neighborIdx] = currentIdx;

                    double newPr = newBestTime + heuristic(neighbor, goal, hero);
                    pq.enqueue(PointWithPriority{ neighbor, newBestTime , newPr});
                }
            }
        }

        return {};
    }
};
