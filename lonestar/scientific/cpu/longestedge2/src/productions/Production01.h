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

  bool execute(const GNode& triangle, Bag* bag) override {
    GNode* gNodes = graph->getGNodesFrom(triangle);

    int edgeToBreakIdx = chooseEdge(triangle->getData(), gNodes);
    if (edgeToBreakIdx < 0) {
      return false;
    }
    const Coordinates& newCoordinates = breakEdge(edgeToBreakIdx, bag, gNodes);
    breakTriangle(edgeToBreakIdx, newCoordinates, gNodes, triangle);
    delete gNodes;
    return true;
  }

private:
  int chooseEdge(const Edge& triangle,
                 GNode* edges) {
    if (!triangle.isTriangle()) {
      return -1;
    }
    if (!triangle.isToRefine() && !isAnyBroken(edges)) {
      return -1;
    }
    const vector<int>& longest = getLongest(edges);
    vector<int> toBreak;
    for (int edge : longest) {
      if (!edges[edge]->getData().isBroken()) {
        toBreak.push_back(edge);
      } else {
        return -1;
      }
    }
    return chooseGreatest(toBreak, edges);
  }
};

#endif // GALOIS_PRODUCTION01_H
