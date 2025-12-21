#pragma once


#include <vector>
#include "../model/Hero.h"

template <typename T>
class PathfinderInterface {
public:
    struct PathWithTime {
        std::vector<T> path;
        double time;
    };

    virtual ~PathfinderInterface() = default;
    virtual PathWithTime findPath(const T& start, const T& goal, Hero& hero, AbstractGraph* graph) = 0;

};
