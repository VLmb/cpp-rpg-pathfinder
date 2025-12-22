#pragma once
#include <memory>

#include "model/CheckpointGraph.h"
#include "model/Grid.h"
#include "path-find/GraphPathfinder.h"
#include "path-find/MapPathfinder.h"
#include "view/MapRenderer.h"


class PathfinderManager {
private:
    std::unique_ptr<Grid> grid;
    std::unique_ptr<CheckpointGraph> graph;
    std::unique_ptr<Perlin2D> perlin;
    std::unique_ptr<MapPathfinder> mapPathfinder;
    std::unique_ptr<GraphPathfinder> graphPathfinder;
    std::unique_ptr<MapRenderer> mapRenderer;
public:
    PathfinderManager(
        std::unique_ptr<Grid> grid,
        std::unique_ptr<CheckpointGraph> graph,
        std::unique_ptr<Perlin2D> perlin,
        std::unique_ptr<MapPathfinder> mapPathfinder,
        std::unique_ptr<GraphPathfinder> graphPathfinder,
        std::unique_ptr<MapRenderer> mapRenderer
        )
    : grid(std::move(grid)),
    graph(std::move(graph)),
    perlin(std::move(perlin)),
    mapPathfinder(std::move(mapPathfinder)),
    graphPathfinder(std::move(graphPathfinder)
    ) {
        this->mapRenderer = std::move(mapRenderer);
    }

    PathfinderManager(
        int width, int height,
        double scale = 0.0f, int seed = 12345) {

        if (width < 5 && height < 5 && scale < 0.0f
            && seed < 0) {
            throw std::invalid_argument("width, height must be greater than 5 and scale greater then 0.");
        }

        perlin = std::make_unique<Perlin2D>(seed);
        auto map = map_generator::generateMap(width, height, perlin.get(), scale);
        grid = std::make_unique<Grid>(map, map_generator::getGridScale(width, height));
        mapPathfinder = std::make_unique<MapPathfinder>(grid.get());
        graph = std::make_unique<CheckpointGraph>(width, height);
        graphPathfinder = std::make_unique<GraphPathfinder>( graph.get(), mapPathfinder.get());
        mapRenderer = std::make_unique<MapRenderer>(map);
    }

    void saveMapToFile(std::string fileName) {
        mapRenderer->saveModifiedMap(fileName);
    }

    bool addVertexToCheckpointGraph(Point checkpoint) {
        if (!graph->addVertex(checkpoint)) {
            return false;
        }
        int multiplier = map_generator::getMultFromCheckpointToPixels(graph->getWidth(), graph->getHeight());
        mapRenderer->drawGraphVertices(
            Point{checkpoint.x * multiplier, checkpoint.y * multiplier},
            multiplier
        );
        return true;
    }

    bool addVertexToCheckpointGraph(int x, int y) {
        return addVertexToCheckpointGraph(Point{x, y});
    }

    bool addEdgeToCheckpointGraph(Point checkpoint1, Point checkpoint2) {
        return graph->addEdge(checkpoint1, checkpoint2);
    }

    bool addEdgeToCheckpointGraph(int x1, int y1, int x2, int y2) {
        return addEdgeToCheckpointGraph(Point{x1,y1}, Point{x2,y2});
    }

    bool removeVertexFromCheckpointGraph(Point checkpoint) {
        return graph->removeVertex(checkpoint);
    }

    bool removeVertexFromCheckpointGraph(int x, int y) {
        return graph->removeVertex(Point{x, y});
    }

    bool removeEdgeFromCheckpointGraph(Point checkpoint1, Point checkpoint2) {
        return graph->removeEdge(checkpoint1, checkpoint2);
    }

    std::vector<Point> findGridPath(Point checkpoint1, Point checkpoint2, Hero& hero) {
        if (!graph->pointIsVertex(checkpoint1) || !graph->pointIsVertex(checkpoint2)) {
            throw std::invalid_argument("Checkpoint graph does not have such vertex.");
        }

        auto pathWithTime = graphPathfinder->findPath(checkpoint1, checkpoint2, hero);

        auto gridPath = mapPathfinder->makeGridPathFromCheckpointsPath(pathWithTime.path, hero);

        return gridPath;
    }

    double findPathAndDraw(Point checkpoint1, Point checkpoint2, Hero& hero, const std::string& fileName) {
        auto pathWithTime = graphPathfinder->findPath(checkpoint1, checkpoint2, hero);
        auto path = pathWithTime.path;
        mapRenderer->drawPath(
            path,
            hero,
            grid->getFactor()
            );
        mapRenderer->saveModifiedMap(fileName);
        return pathWithTime.time;
    }

    double findPathAndDraw(int x1, int y1, int x2, int y2, Hero& hero, const std::string& fileName) {
        return findPathAndDraw(Point{x1, y1}, Point{x2, y2}, hero, fileName);
    }

    void renderAndSaveCurrentMap(const std::string& fileName) const {
        mapRenderer->saveModifiedMap(fileName);
    }

};
