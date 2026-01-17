#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

#include "../data-structure/PriorityQueue.h"
#include "../model/Hero.h"
#include "PathfinderInterface.h"

struct PointWithPriority {
    Point point;
    double timeFromStart;
    double priority;
};

class AStar : public PathfinderInterface<Point> {
private:

    double manhattan(Point a, Point b) const {
        return static_cast<double>(std::abs(a.x - b.x) + std::abs(a.y - b.y));
    }

    double heuristic(Point a, Point b, Hero& hero) const {
        return manhattan(a, b) * hero.getMinTimeToCross();
    }

    std::vector<Point> buildPath(int endIdx, const std::vector<int>& previous, AbstractGraph* graph) const {
        std::vector<Point> path;
        for (int cur = endIdx; cur != -1; cur = previous[cur]) {
            path.push_back(graph->pointOfIndex(cur));
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    virtual double getStepTime(Point cur, Point next, Hero& hero) const = 0;

public:
    AStar() = default;

    PathWithTime findPath(const Point& start, const Point& goal, Hero& hero, AbstractGraph* graph) override {
        if (!graph->inBounds(start) || !graph->inBounds(goal)) {
            throw std::invalid_argument("Start or goal point is out of bounds");
        }

        std::vector<int> previous(graph->getWidth() * graph->getHeight(), -1);
        std::vector<double> bestTimeFromStart(graph->getWidth() * graph->getHeight(), std::numeric_limits<double>::max());

        const int startIdx = graph->indexOf(start);
        const int goalIdx  = graph->indexOf(goal);

        bestTimeFromStart[startIdx] = 0.0;

        PriorityQueue<PointWithPriority> pq([](const PointWithPriority& a, const PointWithPriority& b) -> bool {
            return a.priority < b.priority;
        });

        pq.enqueue(PointWithPriority{ start, 0.0, heuristic(start, goal, hero) });

        while (!pq.isEmpty()) {
            PointWithPriority cur = pq.extractMin();
            const int currentIdx = graph->indexOf(cur.point);

            if (cur.timeFromStart > bestTimeFromStart[currentIdx]) {
                continue;
            }

            if (currentIdx == goalIdx) {
                auto path = buildPath(goalIdx, previous, graph);
                return PathWithTime{path, bestTimeFromStart[currentIdx]};
            }

            for (const auto& neighbor : graph->getNeighbors(currentIdx)) {

                int neighborIdx = graph->indexOf(neighbor);

                double stepTime = getStepTime(cur.point, neighbor, hero);

                double newBestTime = bestTimeFromStart[currentIdx] + stepTime;

                if (newBestTime < bestTimeFromStart[neighborIdx]) {
                    bestTimeFromStart[neighborIdx] = newBestTime;
                    previous[neighborIdx] = currentIdx;

                    double newPr = newBestTime + heuristic(neighbor, goal, hero);
                    pq.enqueue(PointWithPriority{ neighbor, newBestTime , newPr});
                }
            }
        }

        return PathWithTime{ {}, std::numeric_limits<double>::infinity() };

    }
};
