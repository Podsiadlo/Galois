#ifndef GALOIS_PRODUCTION02_H
#define GALOIS_PRODUCTION02_H

//#include <galois/UserContext.h>
#include "../model/Graph.h"
#include "../model/Edge.h"
#include "../model/GraphAdapter.h"
#include "../utils/ProductionHelpers.h"

class Production02 : public Production0x {
public:
  using Production0x::Production0x;

  bool execute(const GNode& triangle, Bag* /*bag*/) override {
    auto gNodes = graph->getGNodesFrom(triangle);
    auto edges  = graph->getEdges(gNodes);

    int toBreak = chooseEdge(triangle->getData(), edges);
    if (toBreak < 0) {
      return false;
    }
    const Coordinates& newCoordinates = getHangingCoordinates(toBreak, gNodes);
    breakTriangle(toBreak, newCoordinates, edges, gNodes, triangle);
    return true;
  }

private:

  const Coordinates& getHangingCoordinates(int brokenEdge,
                                    const vector<GNode>& gNodes) const {
    const vector<GNode>& edgeHalves = graph->getGNodesFrom(gNodes[brokenEdge]);
    auto hangingCoordinates = Edge::getCommonPoint(edgeHalves[0]->getData(),
                                                   edgeHalves[1]->getData());
    return hangingCoordinates.get();
  }


  int chooseEdge(const Edge& triangle,
                 const vector<std::reference_wrapper<Edge>>& edges) {
    if (!triangle.isTriangle()) {
      return -1;
    }
    const vector<int>& longest = getLongest(edges);
    vector<int> toBreak;
    for (auto edge : longest) {
      if (edges[edge].get().isBroken()) {
        toBreak.push_back(edge);
      }
    }
    return chooseGreatest(toBreak, edges);
  }
};

#endif // GALOIS_PRODUCTION02_H
