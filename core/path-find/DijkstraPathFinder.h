#pragma once
#include "../model/Graph.h"
#include "AStarMapPathfinder.h"
#include "GraphPathfinderInterface.h"

class DijkstraPathFinder : public GraphPathfinderInterface<Point> {
private:
    AStarMapPathfinder& mapPathfinder;

    struct PointWithTime {
        Point point;
        double time;
    };

public:
    DijkstraPathFinder(AStarMapPathfinder& mapPathfinder) : mapPathfinder(mapPathfinder) {}



    std::vector<Point> findPath(const Point& start, const Point& goal, Hero& hero, Graph& graph) override {

        PriorityQueue<PointWithTime> pq([](const PointWithTime a, const PointWithTime b) -> bool {
            return a.time > b.time;
        });

        std::vector<Point>
    }


};