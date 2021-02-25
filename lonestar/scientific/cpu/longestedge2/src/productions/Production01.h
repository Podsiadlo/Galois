#ifndef GALOIS_PRODUCTION01_H
#define GALOIS_PRODUCTION01_H

//#include <galois/UserContext.h>
#include "../model/Edge.h"
#include "../model/GraphAdapter.h"
#include "../utils/ProductionHelpers.h"
#include "Production0x.h"

class Production01 : public Production0x {
public:
  using Production0x::Production0x;

  bool execute(const GNode& triangle, Bag* bag) {
    auto gNodes = graph->getGNodesFrom(triangle);
    auto edges  = graph->getEdges(gNodes);

    int edgeToBreakIdx = chooseEdge(triangle->getData(), edges);
    if (edgeToBreakIdx < 0) {
      return false;
    }
    auto newCoordinates = breakEdge(edgeToBreakIdx, bag, edges, gNodes);
    breakTriangle(edgeToBreakIdx, newCoordinates, edges, gNodes, triangle);
    return true;
  }

private:
  int chooseEdge(const Edge& triangle,
                 const vector<std::reference_wrapper<Edge>>& edges) {
    if (!triangle.isTriangle()) {
      return -1;
    }
    if (!triangle.isToRefine() && !isAnyBroken(edges)) {
      return -1;
    }
    const vector<int>& longest = getLongest(edges);
    vector<int> toBreak;
    for (auto edge : longest) {
      if (!edges[edge].get().isBroken()) {
        toBreak.push_back(edge);
      }
    }
    return chooseGreatest(toBreak, edges);
  }
};

#endif // GALOIS_PRODUCTION01_H
