#pragma once

#include "Enums.h"
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

struct Cell {
    TerrainType terrainType;
    float noise;
};

class Grid {
private:
    static const constexpr int MIN_DIMENSION = 3;
    static const constexpr int MAX_DIMENSION = 200;

    int width;
    int height;
    std::vector<Cell> cells;

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

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    Cell getCell(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return cells[indexOf(Point{x, y})];
        } else {
            throw std::out_of_range("Cell coordinates out of range");
        }
    }

    void setCell(int x, int y, const Cell& cell) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            cells[indexOf(Point{x, y})] = cell;
        } else {
            throw std::out_of_range("Cell coordinates out of range");
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
