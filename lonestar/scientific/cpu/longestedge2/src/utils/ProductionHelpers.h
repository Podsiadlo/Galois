#ifndef GALOIS_PRODUCTION_HELPERS_H
#define GALOIS_PRODUCTION_HELPERS_H

#include "../model/Edge.h"

std::vector<int> getLongest(const std::vector<Edge>& edges) {
  std::vector<int> longestEdges;
  double bestLength = 0;
  for (size_t i = 0; i < edges.size(); ++i) {
    auto currentEdge = edges[i];
    double currentLength = currentEdge.getLength();
    if (currentLength - bestLength > EPS) {
      longestEdges.clear();
      longestEdges.emplace_back(i);
      bestLength = currentLength;
    } else if (currentLength - bestLength >= -EPS) {
      longestEdges.emplace_back(i);
    }
  }
  return longestEdges;
}

bool isAnyBroken(const std::vector<Edge> edges) {
  bool result = false;
  for (auto edge: edges) {
    result = result || edge.isBroken();
  }
  return result;
}

int chooseGreatest(std::vector<int> toBreak, const std::vector<Edge>& edges) {
  return 0;//TODO: "less" operator from presentation
}

#endif //GALOIS_PRODUCTION_HELPERS_H