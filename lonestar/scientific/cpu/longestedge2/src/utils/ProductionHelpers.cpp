#include <set>
#include "ProductionHelpers.h"

std::vector<int> getLongest(const vector<GNode>& edges) {
  std::vector<int> longestEdges;
  longestEdges.reserve(3);
  double bestLength = 0;
  for (size_t i = 0; i < edges.size(); ++i) {
    double currentLength = edges[i]->getData().getLength();
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
int chooseGreatest(std::vector<int> toBreak,
                   const vector<GNode>& edges) {
  auto comparator = [&edges](int a, int b) {
    return edges[a]->getData() < edges[b]->getData();
  };
  std::set<int, decltype(comparator)> result(comparator);
  if (toBreak.empty()) {
    return -1;
  }
  for (int & i : toBreak) {
    if (edges[i]->getData().isBorder()) {
      result.insert(i);
    }
  }
  if (result.empty()) {
    result.insert(toBreak.begin(), toBreak.end());
  }
  return *result.rbegin(); //C++ standard guarantees a set is ordered
}

bool isAnyBroken(const std::vector<GNode>& edges) {
  bool result = false;
  for (auto *edge: edges) {
    result = result || edge->getData().isBroken();
  }
  return result;
}
