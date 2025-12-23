#pragma once

#include <cmath>
#include <memory>
#include <vector>

#include "../data-structure/HashMap.h"
#include "../data-structure/PriorityQueue.h"
#include "../model/Grid.h"
#include "../model/Hero.h"
#include "AStar.h"

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

class MapPathfinder : public AStar {
private:
    Grid* grid;
    HashMap<PathKey, PathWithTime, PathKeyHash, PathKeyEq> pathCache;

    double getStepTime(Point cur, Point next, Hero& hero, bool castTerrain = false) const override {
        if (castTerrain) {
            return hero.getTimeToCrossCastTerrain(grid->getCellType(next.x, next.y));
        }
        return hero.getTimeToCross(grid->getCellType(next.x, next.y));
    }

public:
    MapPathfinder(Grid* grid) : grid(grid) {}

    Point getGridCoordinate(Point checkpoint) {
        return Point{
            checkpoint.x * grid->getGridScale(),
            checkpoint.y * grid->getGridScale()
        };
    }

    PathWithTime findPath(const Point& start, const Point& goal, Hero& hero, bool castTerrain = false) {
        if (auto* cached = pathCache.getPtr({ start, goal, &hero })) {
            return *cached;
        }
        auto path = AStar::findPath(start, goal, hero, grid);
        pathCache.put({ start, goal, &hero }, path);

        return path;
    }

    std::vector<Point> makeGridPathFromCheckpointsPath(std::vector<Point> checkpoints, Hero& hero) {
        std::vector<Point> gridPath;

        if (checkpoints.size() < 2) {
            return {};
        }

        for (int i = 0; i < checkpoints.size() - 1; i++) {
            auto checkpoint1 = getGridCoordinate(checkpoints[i]);
            auto checkpoint2 = getGridCoordinate(checkpoints[i + 1]);
            auto partGripPath = findPath(checkpoint1, checkpoint2, hero).path;
            gridPath.insert(gridPath.end(), partGripPath.begin(), partGripPath.end());
        }

        return gridPath;
    }

    std::vector<Point> getCachedGridPath(Point start, Point goal, Hero& hero) {
        if (auto* cached = pathCache.getPtr(
            PathKey{
            getGridCoordinate(start),
            getGridCoordinate(goal),
            &hero })) {
            return cached->path;
        }
        return {};
    }

    bool isGridPathCached(Point start, Point goal, Hero& hero) {
        return (pathCache.getPtr(
            PathKey{
            getGridCoordinate(start),
            getGridCoordinate(goal),
            &hero })
            != nullptr);
    }

    bool removeGridPathFromCache(Point start, Point goal, Hero& hero) {
        return pathCache.remove(
            PathKey{
                getGridCoordinate(start),
                getGridCoordinate(goal),
                &hero
            });
    }

    std::vector<const PathWithTime*> getAllCachedGridPathWithTime() {
        return pathCache.valuesPtr();
    }
};
