#pragma once
#include <memory>

#include "model/Grid.h"
#include "path-find/AStarGraphPathFinder.h"
#include "path-find/AStarMapPathfinder.h"

class PathfinderManager {
private:
    std::unique_ptr<Grid> grid;
    std::unique_ptr<CheckpointGraph> graph;
    std::unique_ptr<Perlin2D> perlin;
    std::unique_ptr<AStarMapPathfinder> mapPathfinder;
    std::unique_ptr<AStarGraphPathFinder> graphPathfinder;
public:
    PathfinderManager(
        std::unique_ptr<Grid> grid,
        std::unique_ptr<Perlin2D> perlin,
        std::unique_ptr<AStarMapPathfinder> mapPathfinder,
        std::unique_ptr<AStarGraphPathFinder> graphPathfinder
        )
    : grid(std::move(grid)),
    perlin(std::move(perlin)),
    mapPathfinder(std::move(mapPathfinder)),
    graphPathfinder(std::move(graphPathfinder)) {}

    PathfinderManager(
        int width, int height,
        double scale = 0.0f, int seed = 12345) {

        if (width < 5 && height < 5 && scale < 0.0f
            && seed < 0) {
            throw std::invalid_argument("width, height must be greater than 5 and scale greater then 0.");
        }

        perlin = std::make_unique<Perlin2D>(seed);
        auto map = map_generator::GenerateMap(width, height, perlin.get(), scale);
        grid = std::make_unique<Grid>(map, map_generator::getGridScale(width, height));
        mapPathfinder = std::make_unique<AStarMapPathfinder>(grid.get());
        graph = std::make_unique<CheckpointGraph>(width, height);
        graphPathfinder = std::make_unique<AStarGraphPathFinder>(mapPathfinder.get(), graph.get());
    }
};
