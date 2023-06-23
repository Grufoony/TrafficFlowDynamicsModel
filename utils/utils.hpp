#ifndef UTILS_HPP
#define UTILS_HPP

// Some useful functions for the project

#include <limits>
#include <map>
#include <vector>

// function for dijkstra which returns the index of the vertex with minimum
// distance from the source
inline int minDistance(std::vector<int> const &dist, std::vector<bool> const &sptSet,
                int const _n) {
  // Initialize min value
  int min = std::numeric_limits<int>::max(), min_index = -1;

  for (int v = 0; v < _n; ++v) {
    if (!sptSet[v] && dist[v] <= min) {
      min = dist[v], min_index = v;
    }
  }
  return min_index;
}

// function which normalizes vector components to one
inline void normalizeVec(std::vector<double> &vec) {
  double sum = 0.;
  for (auto it : vec)
    sum += it;
  if (static_cast<int>(sum) == 0)
    return;
  for (auto &it : vec)
    it = it / sum;
}

#endif