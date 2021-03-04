#include <set>
#include "ProductionHelpers.h"

std::vector<int> getLongest(const vector<std::reference_wrapper<Edge>>& edges) {
  std::vector<int> longestEdges;
  double bestLength = 0;
  for (size_t i = 0; i < edges.size(); ++i) {
    auto currentEdge = edges[i].get();
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
int chooseGreatest(std::vector<int> toBreak,
                   const vector<std::reference_wrapper<Edge>>& edges) {
  auto comparator = [&edges](int a, int b) {
    return edges[a].get() < edges[b].get();
  };
  std::set<int, decltype(comparator)> result(comparator);
  if (edges.empty()) {
    return -1;
  }
  for (int & i : toBreak) {
    if (edges[i].get().isBorder()) {
      result.insert(i);
    }
  }
  if (result.empty()) {
    result.insert(toBreak.begin(), toBreak.end());
  }
  return *result.rbegin(); //C++ standard guarantees a set is ordered
}

bool isAnyBroken(const std::vector<std::reference_wrapper<Edge>>& edges) {
  bool result = false;
  for (auto edge: edges) {
    result = result || edge.get().isBroken();
  }
  return result;
}
