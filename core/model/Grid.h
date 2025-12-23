#pragma once

#include <stdexcept>
#include <vector>
#include "../map-generation/MapGenerator.h"
#include "AbstractGraph.h"

struct CellProperty {
    float temperatureAvg;
    float altitudeAvg;
    float moistureAvg;

    float castTemperatureAvg;
    float castAltitudeAvg;
    float castMoistureAvg;
};

class Grid : public AbstractGraph {
private:
    static constexpr int MIN_DIMENSION = 5;
    static constexpr int MAX_DIMENSION = 400;

    int grid_scale;
    std::vector<CellProperty> cells;
    std::vector<std::vector<map_generator::TerrainType>> map;

public:
    Grid(int width, int height, const int grid_scale = 1)
        : AbstractGraph(width, height), grid_scale(grid_scale) {
        if (width < MIN_DIMENSION || height < MIN_DIMENSION) {
            throw std::invalid_argument("Grid dimensions must be greater than 2");
        }
        if (width > MAX_DIMENSION || height > MAX_DIMENSION) {
            throw std::invalid_argument("Grid dimensions must be less than or equal to 400");
        }
        cells.resize(height * width);
    }

    Grid(std::vector<std::vector<map_generator::TerrainType>>& map, const int grid_scale = 1)
        : Grid(static_cast<int>(map[0].size()) / map_generator::GRID_FACTOR, static_cast<int>(map.size()) / map_generator::GRID_FACTOR, grid_scale) {

        this->map = map;
        this->grid_scale = grid_scale;

        const int srcH = static_cast<int>(map.size());
        const int srcW = static_cast<int>(map[0].size());

        if (srcW < 3 || srcH < 3)
            throw std::invalid_argument("Source map must be at least 3x3 for 3x3 downsampling");

        for (int srcY = 0; srcY + 2 < srcH; srcY += 3) {
            for (int srcX = 0; srcX + 2 < srcW; srcX += 3) {

                float T = 0.0f, A = 0.0f, M = 0.0f;
                float cT = 0.0f, cA = 0.0f, cM = 0.0f;

                for (int dy = 0; dy < map_generator::GRID_FACTOR; ++dy) {
                    for (int dx = 0; dx < map_generator::GRID_FACTOR; ++dx) {
                        const auto& s = map[srcY + dy][srcX + dx];
                        T += s.temperature;
                        A += s.altitude;
                        M += s.moisture;
                        cT += s.castTemperature;
                        cA += s.castAltitude;
                        cM += s.castMoisture;
                    }
                }

                const int x = srcX / map_generator::GRID_FACTOR;
                const int y = srcY / map_generator::GRID_FACTOR;

                CellProperty c;
                c.temperatureAvg = T / static_cast<float>(map_generator::GRID_FACTOR * map_generator::GRID_FACTOR);
                c.altitudeAvg = A / static_cast<float>(map_generator::GRID_FACTOR * map_generator::GRID_FACTOR);
                c.moistureAvg = M / static_cast<float>(map_generator::GRID_FACTOR * map_generator::GRID_FACTOR);
                c.castTemperatureAvg = cT / static_cast<float>(map_generator::GRID_FACTOR * map_generator::GRID_FACTOR);
                c.castAltitudeAvg = cA / static_cast<float>(map_generator::GRID_FACTOR * map_generator::GRID_FACTOR);
                c.castMoistureAvg = cM / static_cast<float>(map_generator::GRID_FACTOR * map_generator::GRID_FACTOR);

                cells[indexOf(Point{ x, y })] = c;
            }
        }
    }

    CellProperty getCellType(int x, int y) const {
        if (inBounds(x, y)) {
            return cells[indexOf(Point{ x, y })];
        }
        throw std::out_of_range("CellType coordinates out of range");
    }

    void setCellType(int x, int y, const CellProperty& cell) {
        if (inBounds(x, y)) {
            cells[indexOf(Point{ x, y })] = cell;
            return;
        }
        throw std::out_of_range("CellType coordinates out of range");
    }

    std::vector<Point> getNeighbors(int idx) const override {
        if (idx < 0 || idx >= width * height) {
            throw std::out_of_range("Grid::getNeighbors: idx out of bounds");
        }

        Point p = pointOfIndex(idx);
        const Point directions[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

        std::vector<Point> neighbors;
        neighbors.reserve(4);

        for (const auto& dir : directions) {
            Point next{ p.x + dir.x, p.y + dir.y };
            if (inBounds(next)) {
                neighbors.push_back(next);
            }
        }

        return neighbors;
    }

    int getGridScale() const {
        return grid_scale;
    }

    int getGridFactor() const {
        return map_generator::GRID_FACTOR;
    }
};
