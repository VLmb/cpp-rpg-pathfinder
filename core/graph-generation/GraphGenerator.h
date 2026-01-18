#pragma once
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "../model/CheckpointGraph.h"


class GraphGenerator {
private:

    struct GridConfig {
        int cols, rows;
        int stepX, stepY;
    };

    struct GenPoint {
        Point p;
        bool exists;
    };

    using GridMap = std::vector<std::vector<GenPoint>>;

    static float randomRange(float min, float max) {
        float randomFloat = static_cast<float>(std::rand()) / RAND_MAX;
        return min + randomFloat * (max - min);
    }

    static bool randomChance(double probability) {
        if (probability >= 1.0) return true;
        if (probability <= 0.0) return false;
        return (static_cast<double>(std::rand()) / RAND_MAX) <= probability;
    }

    static GridConfig calculateGridConfig(int width, int height, int targetVertices) {
        float ratio = (float)width / height;
        int rows = std::max(1, (int)std::sqrt(targetVertices / ratio));
        int cols = std::max(1, (int)(targetVertices / (float)rows));
        return { cols, rows, width / cols, height / rows };
    }

    static Point getJitteredPoint(int col, int row, const GridConfig& cfg, float jitter) {
        int centerX = col * cfg.stepX + cfg.stepX / 2;
        int centerY = row * cfg.stepY + cfg.stepY / 2;

        float offsetX = randomRange(-jitter, jitter) * cfg.stepX;
        float offsetY = randomRange(-jitter, jitter) * cfg.stepY;

        return { centerX + (int)offsetX, centerY + (int)offsetY };
    }

    static GridMap placeVertices(CheckpointGraph& graph, const GridConfig& cfg, float jitter) {
        GridMap grid(cfg.cols, std::vector<GenPoint>(cfg.rows));

        for (int c = 0; c < cfg.cols; ++c) {
            for (int r = 0; r < cfg.rows; ++r) {
                Point p = getJitteredPoint(c, r, cfg, jitter);

                if (graph.addVertex(p)) {
                    grid[c][r] = { p, true };
                } else {
                    grid[c][r] = { {0,0}, false };
                }
            }
        }
        return grid;
    }

    static void connectNeighbors(CheckpointGraph& graph, const GridMap& grid, const GridConfig& cfg, double prob) {
        for (int c = 0; c < cfg.cols; ++c) {
            for (int r = 0; r < cfg.rows; ++r) {
                if (!grid[c][r].exists) continue;

                Point currentP = grid[c][r].p;

                auto tryConnect = [&](int nc, int nr) {
                    if (nc >= cfg.cols || nr >= cfg.rows) return;

                    const auto& neighbor = grid[nc][nr];
                    if (neighbor.exists && randomChance(prob)) {
                        graph.addEdge(currentP, neighbor.p);
                    }
                };

                tryConnect(c + 1, r);
                tryConnect(c, r + 1);
            }
        }
    }

public:

    static void generate(CheckpointGraph& graph, int targetVertices, float jitter = 0.5f, double connProb = 0.8) {
        std::srand(std::time(nullptr));

        graph.reset();

        GridConfig cfg = calculateGridConfig(graph.getWidth(), graph.getHeight(), targetVertices);
        GridMap gridMap = placeVertices(graph, cfg, jitter);
        connectNeighbors(graph, gridMap, cfg, connProb);
    }
};