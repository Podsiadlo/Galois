#ifndef GALOIS_PRODUCTION02_H
#define GALOIS_PRODUCTION02_H

//#include <galois/UserContext.h>
#include "../model/Graph.h"
#include "../model/GraphAdapter.h"
#include "../utils/ProductionHelpers.h"

class Production02 : public Production0x {
public:
  using Production0x::Production0x;

  bool execute(const GNode& triangle, Bag* /*bag*/) override {
    GNode* gNodes = graph->getGNodesFrom(triangle);

    int toBreak = chooseEdge(triangle->getData(), gNodes);
    if (toBreak < 0) {
      return false;
    }
    const Coordinates& newCoordinates = getHangingCoordinates(toBreak, gNodes);
    breakTriangle(toBreak, newCoordinates, gNodes, triangle);
    delete gNodes;
    return true;
  }

private:

  const Coordinates& getHangingCoordinates(int brokenEdge,
                                    const GNode* gNodes) const {
    GNode* edgeHalves = graph->getGNodesFrom(gNodes[brokenEdge]);
    auto hangingCoordinates = Edge::getCommonPoint(edgeHalves[0]->getData(),
                                                   edgeHalves[1]->getData());
    delete edgeHalves;
    return hangingCoordinates.get();
  }


  int chooseEdge(const Edge& triangle,
                 const GNode* edges) {
    if (!triangle.isTriangle()) {
      return -1;
    }
    const vector<int>& longest = getLongest(edges);
    vector<int> toBreak;
    for (auto edge : longest) {
      if (edges[edge]->getData().isBroken()) {
        toBreak.push_back(edge);
      }
    }
    return chooseGreatest(toBreak, edges);
  }
};

#endif // GALOIS_PRODUCTION02_H
