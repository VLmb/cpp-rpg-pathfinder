#pragma once

#include "../map-generation/MapGenerator.h"
#include <vector>
#include <stdexcept>
#include "AbstractGraph.h"

struct CellProperty {
    float rockinessAvg;
    float vegetationAvg;
    float moistureAvg;
};

class Grid : public AbstractGraph {
private:
    static constexpr int MIN_DIMENSION = 3;
    static constexpr int MAX_DIMENSION = 200;

    int grid_scale;
    int factor = map_generator::GRID_FACTOR;
    std::vector<CellProperty> cells;
    std::vector<std::vector<map_generator::TerrainType>> map;

public:
    Grid(int width, int height, const int grid_scale = 1)
        : AbstractGraph(width, height), grid_scale(grid_scale) {
        if (width < MIN_DIMENSION || height < MIN_DIMENSION) {
            throw std::invalid_argument("Grid dimensions must be greater than 2");
        }
        if (width > MAX_DIMENSION || height > MAX_DIMENSION) {
            throw std::invalid_argument("Grid dimensions must be less than or equal to 200");
        }
        cells.resize(height * width);
    }

    Grid(std::vector<std::vector<map_generator::TerrainType>>& map, const int grid_scale = 1)
        : Grid(static_cast<int>(map[0].size()) / 3, static_cast<int>(map.size()) / 3, grid_scale) {

        this->map = map;
        this->grid_scale = grid_scale;

        const int srcH = static_cast<int>(map.size());
        const int srcW = static_cast<int>(map[0].size());

        if (srcW < 3 || srcH < 3)
            throw std::invalid_argument("Source map must be at least 3x3 for 3x3 downsampling");

        for (int srcY = 0; srcY + 2 < srcH; srcY += 3) {
            for (int srcX = 0; srcX + 2 < srcW; srcX += 3) {

                float r = 0.0f, v = 0.0f, m = 0.0f;

                for (int dy = 0; dy < 3; ++dy) {
                    for (int dx = 0; dx < 3; ++dx) {
                        const auto& s = map[srcY + dy][srcX + dx];
                        r += s.rockiness;
                        v += s.vegetation;
                        m += s.moisture;
                    }
                }

                const int x = srcX / 3;
                const int y = srcY / 3;

                CellProperty c;
                c.rockinessAvg = r / 9.0f;
                c.vegetationAvg = v / 9.0f;
                c.moistureAvg = m / 9.0f;

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
};
