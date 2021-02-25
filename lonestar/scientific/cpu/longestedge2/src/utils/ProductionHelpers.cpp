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
  std::vector<int> result{};
  if (edges.size() == 0) {
    return -1;
  }
  for (auto i = toBreak.begin(); i!=toBreak.end() ; ++i) {
    if (edges[*i].get().isBorder()) {
      result.emplace_back(*i);
    }
  }
  if (result.size() > 1 || result.size() == 0) {
    //TODO: porownaj wspolrzedne
  } else {
    return result[0];
  }
  return toBreak[0];
}

bool isAnyBroken(std::vector<std::reference_wrapper<Edge>> edges) {
  bool result = false;
  for (auto edge: edges) {
    result = result || edge.get().isBroken();
  }
  return result;
}
