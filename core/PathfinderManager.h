#pragma once
#include <memory>

#include "model/CheckpointGraph.h"
#include "model/Grid.h"
#include "path-find/GraphPathfinder.h"
#include "path-find/MapPathfinder.h"
#include "view/MapRenderer.h"
#include "model/Hero.h"


class PathfinderManager {
private:
    static constexpr int MIN_DIMENSION = 1;
    static constexpr int MAX_DIMENSION = 400;

    std::unique_ptr<Grid> grid;
    std::unique_ptr<CheckpointGraph> graph;
    std::unique_ptr<Perlin2D> perlin;
    std::unique_ptr<MapPathfinder> mapPathfinder;
    std::unique_ptr<GraphPathfinder> graphPathfinder;
    std::unique_ptr<MapRenderer> mapRenderer;

    std::string fileName;

    bool isCastMap;

    int getMultiplier(Point checkpoint) {
        return map_generator::getMultFromCheckpointToPixels(graph->getWidth(), graph->getHeight());
    }

    Point getMappedPixelsCoordinate(Point checkpoint) {
        int multiplier = getMultiplier(checkpoint);
        return Point{checkpoint.x * multiplier, checkpoint.y * multiplier};
    }
public:
    PathfinderManager(
        std::unique_ptr<Grid> grid,
        std::unique_ptr<CheckpointGraph> graph,
        std::unique_ptr<Perlin2D> perlin,
        std::unique_ptr<MapPathfinder> mapPathfinder,
        std::unique_ptr<GraphPathfinder> graphPathfinder,
        std::unique_ptr<MapRenderer> mapRenderer,
        const std::string& filename,
        bool isCastMap = false
        )
    : grid(std::move(grid)),
    graph(std::move(graph)),
    perlin(std::move(perlin)),
    mapPathfinder(std::move(mapPathfinder)),
    fileName(filename),
    graphPathfinder(std::move(graphPathfinder)
    ) {
        this->mapRenderer = std::move(mapRenderer);
    }

    PathfinderManager(
        int width, int height, const std::string& filename, bool isCastMap = false,
        double scale = 0.0f, int seed = 12345) {

        if (width < MIN_DIMENSION &&
            height < MIN_DIMENSION &&
            scale < 0.0f &&
            seed < 0)
        {
            throw std::invalid_argument("width, height must be greater than 5 and scale greater then 0.");
        }

        perlin = std::make_unique<Perlin2D>(seed);
        auto map = isCastMap
                ? map_generator::generateCastMap(width, height, perlin.get(), scale)
                : map_generator::generateNaturalMap(width, height, perlin.get(), scale);
        grid = std::make_unique<Grid>(map, map_generator::getGridScale(width, height));
        mapPathfinder = std::make_unique<MapPathfinder>(grid.get());
        graph = std::make_unique<CheckpointGraph>(width, height);
        graphPathfinder = std::make_unique<GraphPathfinder>( graph.get(), mapPathfinder.get());
        mapRenderer = std::make_unique<MapRenderer>(map);

        fileName = filename;
    }

    void saveMapToFile() {
        mapRenderer->saveModifiedMap(fileName);
    }

    bool addVertexToCheckpointGraph(Point checkpoint) {
        if (!graph->addVertex(checkpoint)) {
            return false;
        }
        mapRenderer->drawGraphVertices(
            getMappedPixelsCoordinate(checkpoint),
            getMultiplier(checkpoint)
        );
        mapRenderer->saveModifiedMap(fileName);
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
        bool f = graph->removeVertex(checkpoint);
        mapRenderer->undrawGraphVertices(
            getMappedPixelsCoordinate(checkpoint),
            getMultiplier(checkpoint)
            );
        mapRenderer->saveModifiedMap(fileName);
        return f;
    }

    bool removeVertexFromCheckpointGraph(int x, int y) {
        return removeVertexFromCheckpointGraph(Point{x, y});
    }

    bool removeEdgeFromCheckpointGraph(Point checkpoint1, Point checkpoint2) {
        bool f = graph->removeEdge(checkpoint1, checkpoint2);
        mapRenderer->saveModifiedMap(fileName);
        return f;
    }

    bool removeEdgeFromCheckpointGraph(int x1, int y1, int x2, int y2) {
        return removeEdgeFromCheckpointGraph(Point{x1,y1}, Point{x2,y2});
    }

    std::vector<Point> findGridPath(Point checkpoint1, Point checkpoint2, Hero& hero, bool castTerrain = false) {
        if (!graph->pointIsVertex(checkpoint1) || !graph->pointIsVertex(checkpoint2)) {
            throw std::invalid_argument("Checkpoint graph does not have such vertex.");
        }

        auto pathWithTime = graphPathfinder->findPath(checkpoint1, checkpoint2, hero, castTerrain);

        auto gridPath = mapPathfinder->makeGridPathFromCheckpointsPath(pathWithTime.path, hero);

        return gridPath;
    }

    double findPathAndDraw(Point checkpoint1, Point checkpoint2, Hero& hero, bool castTerrain = false) {
        auto pathWithTime = graphPathfinder->findPath(checkpoint1, checkpoint2, hero, castTerrain);
        auto path = pathWithTime.path;
        auto gridPath = mapPathfinder->makeGridPathFromCheckpointsPath(path, hero);
        mapRenderer->drawPath(
            gridPath,
            hero,
            grid->getGridFactor()
            );
        mapRenderer->saveModifiedMap(fileName);
        return pathWithTime.time;
    }

    double findPathAndDraw(int x1, int y1, int x2, int y2, Hero& hero, bool castTerrain = false) {
        return findPathAndDraw(Point{x1, y1}, Point{x2, y2}, hero);
    }

    void renderAndSaveCurrentMap(const std::string& fileName) const {
        mapRenderer->saveModifiedMap(fileName);
    }

    std::vector<std::vector<int>> getCheckpointGraphAdjList() const {
        return graph->getAdjList();
    }

    Point getPointOfIndex(int idx) const {
        return graph->pointOfIndex(idx);
    }

    int getIndexOfPoint(int x, int y) const {
        return graph->indexOf(x, y);
    }

    void cleanMap() {
        mapRenderer->cleanMap(fileName);
    }

    void drawAllCheckpoints() {
        for (auto checkpoint: graph->getVerticesList()) {
            mapRenderer->drawGraphVertices(
            getMappedPixelsCoordinate(checkpoint),
            getMultiplier(checkpoint)
            );
        }
    }
};
