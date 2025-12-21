#pragma once

#include "../model/Graph.h"

#include "../model/Hero.h"
#include <vector>

template <typename T>
class GraphPathfinderInterface {
public:
  virtual ~GraphPathfinderInterface() = default;
  virtual std::vector<T> findPath(const T& start, const T& goal, Hero& hero, Graph& graph) = 0;

};
