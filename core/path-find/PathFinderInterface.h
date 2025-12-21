#pragma once

template <typename T>
class PathFinderInterface {
public:
    virtual std::vector<T> findPath(const T& start, const T& goal, Hero& hero) = 0;
};