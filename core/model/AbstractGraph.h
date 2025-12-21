#pragma once

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

class AbstractGraph {
protected:
    int width;
    int height;

    AbstractGraph(int width, int height) : width(width), height(height) {}

public:
    virtual ~AbstractGraph() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    bool inBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    bool inBounds(Point p) const {
        return inBounds(p.x, p.y);
    }

    int indexOf(int x, int y) const {
        if (!inBounds(x, y)) throw std::out_of_range("AbstractGraph::indexOf: point out of bounds");
        return x + y * width;
    }

    int indexOf(Point p) const {
        return indexOf(p.x, p.y);
    }

    Point pointOfIndex(int idx) const {
        if (idx < 0 || idx >= width * height) throw std::out_of_range("AbstractGraph::pointOfIndex: idx out of bounds");
        return Point{ idx % width, idx / width };
    }

    virtual std::vector<Point> getNeighbors(int idx) const = 0;
};
