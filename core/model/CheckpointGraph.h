#pragma once

#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <algorithm>
#include <cstdint>

#include "AbstractGraph.h"

class CheckpointGraph : public AbstractGraph {
private:
    std::vector<bool> isVertex;
    std::vector<std::vector<int>> adjList;

    bool hasNeighbor(const std::vector<int>& neighbors, int index) const {
        return std::find(neighbors.begin(), neighbors.end(), index) != neighbors.end();
    }

public:
    CheckpointGraph(int width, int height)
        : AbstractGraph(width, height), isVertex(width * height), adjList(width * height) {}

    bool addVertex(Point point) {
        if (inBounds(point)) {
            isVertex[indexOf(point)] = true;
            return true;
        }

        return false;
    }

    bool removeVertex(Point point) {
        if (inBounds(point)) {
            int idx = indexOf(point);
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
        if (inBounds(a) && inBounds(b)) {
            int indexA = indexOf(a);
            int indexB = indexOf(b);

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
        if (inBounds(a) && inBounds(b)) {
            int indexA = indexOf(a);
            int indexB = indexOf(b);

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
        if (inBounds(a) && inBounds(b)) {
            int indexA = indexOf(a);
            int indexB = indexOf(b);

            if (isVertex[indexA] && isVertex[indexB]) {
                return hasNeighbor(adjList[indexA], indexB);
            }
        }

        return false;
    }
    
    bool neighborsIdx(Point v, std::vector<int>& result) const {
        if (inBounds(v)) {
            int idx = indexOf(v);
            if (isVertex[idx]) {
                result = adjList[idx];
                return true;
            }
        }

        return false;
    }

    bool neighbors(Point v, std::vector<Point>& result) const {
        if (inBounds(v)) {
            int idx = indexOf(v);
            if (isVertex[idx]) {
                for (int neighborIdx : adjList[idx]) {
                    result.push_back(pointOfIndex(neighborIdx));
                }
                return true;
            }
        }

        return false;;
    }

    std::vector<Point> getNeighbors(int idx) const override {
        if (idx < 0 || idx >= width * height) {
            throw std::out_of_range("CheckpointGraph::getNeighbors: idx out of bounds");
        }

        std::vector<Point> result;
        if (!isVertex[idx]) return result;

        for (int neighborIdx : adjList[idx]) {
            result.push_back(pointOfIndex(neighborIdx));
        }

        return result;
    }

    std::vector<std::vector<int>> getAdjList() const { return adjList; }
};
