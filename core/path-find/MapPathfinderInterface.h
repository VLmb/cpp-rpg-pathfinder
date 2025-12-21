#pragma once

#include <vector>
#include "../model/Hero.h"

template <typename T>
class MapPathfinderInterface {
public:
    virtual ~MapPathfinderInterface() = default;
    virtual std::vector<T> findPath(const T& start, const T& goal, Hero& hero) = 0;
};