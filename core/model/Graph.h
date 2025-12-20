#pragma once

#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <algorithm>
#include <cstdint>

#include "Grid.h"

class Graph {

private:
    int width;
    int height;
    std::vector<bool> isVertex;
    std::vector<std::vector<int>> adjList;

    bool isBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    int indexOf(int x, int y) const {
        return x + y * width;
    }

    Point point(int index) const {
        return Point{ index % width, index / width };
    }

    bool hasNeighbor(const std::vector<int>& neighbors, int index) const {
        return std::find(neighbors.begin(), neighbors.end(), index) != neighbors.end();
    }

public:
    Graph(int width, int height) : width(width), height(height), isVertex(width * height), adjList(width * height) {}

    bool addVertex(Point point) {
        if (isBounds(point.x, point.y)) {
            isVertex[indexOf(point.x, point.y)] = true;
            return true;
        }

        return false;
    }

    bool removeVertex(Point point) {
        if (isBounds(point.x, point.y)) {
            int idx = indexOf(point.x, point.y);
            for (int nb : adjList[idx]) {
                auto& v = adjList[nb];
                v.erase(std::remove(v.begin(), v.end(), idx), v.end());
            }
            adjList[idx].clear();
            isVertex[idx] = false;
            return true;
        }

        return false;
    }

    bool addEdge(Point a, Point b) {
        if (isBounds(a.x, a.y) && isBounds(b.x, b.y)) {
            int indexA = indexOf(a.x, a.y);
            int indexB = indexOf(b.x, b.y);

            if (indexA == indexB) return false;

            if (isVertex[indexA] && isVertex[indexB] 
                && !hasNeighbor(adjList[indexA], indexB) 
                && !hasNeighbor(adjList[indexB], indexA)) {
                adjList[indexA].push_back(indexB);
                adjList[indexB].push_back(indexA);
                return true;
            }
        }

        return false;
    }

    bool removeEdge(Point a, Point b) {
        if (isBounds(a.x, a.y) && isBounds(b.x, b.y)) {
            int indexA = indexOf(a.x, a.y);
            int indexB = indexOf(b.x, b.y);

            if (isVertex[indexA] && isVertex[indexB]
                && hasNeighbor(adjList[indexA], indexB)
                && hasNeighbor(adjList[indexB], indexA)) {
                adjList[indexA].erase(std::find(adjList[indexA].begin(), adjList[indexA].end(), indexB));
                adjList[indexB].erase(std::find(adjList[indexB].begin(), adjList[indexB].end(), indexA));
                return true;
            }
        }

        return false;
    }

    bool hasEdge(Point a, Point b) const {
        if (isBounds(a.x, a.y) && isBounds(b.x, b.y)) {
            int indexA = indexOf(a.x, a.y);
            int indexB = indexOf(b.x, b.y);

            if (isVertex[indexA] && isVertex[indexB]) {
                return hasNeighbor(adjList[indexA], indexB);
            }
        }

        return false;
    }
    
    bool neighborsIdx(Point v, std::vector<int>& result) const {
        if (isBounds(v.x, v.y)) {
            int idx = indexOf(v.x, v.y);
            if (isVertex[idx]) {
                result = adjList[idx];
                return true;
            }
        }

        return false;
    }

    bool neighbors(Point v, std::vector<Point>& result) const {
        if (isBounds(v.x, v.y)) {
            int idx = indexOf(v.x, v.y);
            if (isVertex[idx]) {
                for (int neighborIdx : adjList[idx]) {
                    result.push_back(point(neighborIdx));
                }
                return true;
            }
        }

        return false;;
    }

};