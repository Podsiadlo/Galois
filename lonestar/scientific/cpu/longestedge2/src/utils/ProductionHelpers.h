#ifndef GALOIS_PRODUCTION_HELPERS_H
#define GALOIS_PRODUCTION_HELPERS_H

#include "../model/Edge.h"

std::vector<int> getLongest(const std::vector<Edge>& edges) {
  std::vector<Edge> longest;
  double record = 0;
  for(auto edge: edges) {
    if (edge.getLength() - record > EPS) {
      longest.clear();
      longest.emplace_back(edge);
      record = edge.getLength();
    } else if (edge.getLength() - record >= -EPS) {
      longest.emplace_back(edge);
    }
  }
  return longest;
}

bool isAnyBroken(const std::vector<Edge> edges) {
  bool result = false;
  for (auto edge: edges) {
    result = result || edge.isBroken();
  }
  return result;
}

int chooseGreatest(std::vector<int> toBreak, const std::vector<Edge>& edges) {
  return 0;
}

#endif //GALOIS_PRODUCTION_HELPERS_H