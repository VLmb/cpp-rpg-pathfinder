#pragma once
#include <memory>


#include "../model/CheckpointGraph.h"
#include "AStar.h"
#include "MapPathfinder.h"


class GraphPathfinder : public AStar {
private:
    CheckpointGraph* graph;
    MapPathfinder* mapPathfinder;

    struct PointWithTime {
        Point point;
        double time;
    };

    double getStepTime(Point cur, Point next, Hero &hero, bool castTerrain = false) const override {
        return (mapPathfinder->findPath(
            mapPathfinder->getGridCoordinate(cur),
            mapPathfinder->getGridCoordinate(next),
            hero,
            castTerrain
            )).time;
    }

public:
    GraphPathfinder(CheckpointGraph* graph, MapPathfinder* mapPathfinder) : graph(graph), mapPathfinder(mapPathfinder) {}

    PathWithTime findPath(const Point &start, const Point &goal, Hero &hero, bool castTerrain = false) {
        if (!graph->pointIsVertex(start) || !graph->pointIsVertex(goal)) {
            throw std::invalid_argument("Start and goal are invalid");
        }
        return AStar::findPath(start, goal, hero, graph, castTerrain);
    }
};