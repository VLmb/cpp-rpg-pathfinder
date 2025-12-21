#pragma once

#include "../map-generation/MapGenerator.h"
#include <vector>
#include <stdexcept>

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

struct CellType {
    float rockinessAvg;   
    float vegetationAvg;  
    float moistureAvg; 
};

class Grid {
private:
    static constexpr int MIN_DIMENSION = 3;
    static constexpr int MAX_DIMENSION = 200;

    int width;
    int height;
    std::vector<CellType> cells;

public:
    Grid(int width, int height) {
        if (width < MIN_DIMENSION || height < MIN_DIMENSION) {
            throw std::invalid_argument("Grid dimensions must be greater than 2");
        }
        if (width > MAX_DIMENSION || height > MAX_DIMENSION) {
            throw std::invalid_argument("Grid dimensions must be less than or equal to 200");
        }
        this->width = width;
        this->height = height;
        cells.resize(height * width);
    }

    Grid(std::vector<std::vector<TerrainType>>& map)
     : Grid(static_cast<int>(map[0].size()) / 3, static_cast<int>(map.size()) / 3) {

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

                CellType c;
                c.rockinessAvg  = r / 9.0f;
                c.vegetationAvg = v / 9.0f;
                c.moistureAvg   = m / 9.0f;

                cells[indexOf(Point{x, y})] = c;
            }
        }
    }

    static Grid createGrid(int width, int height, double scale = 0.0f) {

    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    CellType getCellType(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return cells[indexOf(Point{x, y})];
        } else {
            throw std::out_of_range("CellType coordinates out of range");
        }
    }

    void setCellType(int x, int y, const CellType& cell) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            cells[indexOf(Point{x, y})] = cell;
        } else {
            throw std::out_of_range("CellType coordinates out of range");
        }
    }

    bool inBounds(Point p) const {
        return p.x >= 0 && p.y >= 0 && p.x < width && p.y < height;
    }

    int indexOf(Point p) const {
        if (!inBounds(p)) throw std::out_of_range("Grid::indexOf: point out of bounds");
        return p.y * width + p.x;
    }

    Point pointOfIndex(int idx) const {
        if (idx < 0 || idx >= width * height) throw std::out_of_range("Grid::pointOfIndex: idx out of bounds");
        return Point{ idx % width, idx / width };
    }

};
