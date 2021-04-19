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
    //    vector<GNode> gNodes = graph->getGNodesFrom(triangle);
    std::vector<GNode> gNodes;
    gNodes.reserve(3);
    auto outEdges = graph->getGraph()->out_edges(triangle);
    std::transform(
        outEdges.begin(), outEdges.end(), std::back_inserter(gNodes),
        [&](auto& edge) { return graph->getGraph()->getEdgeDst(edge); });

    int toBreak = chooseEdge(triangle->getData(), gNodes);
    if (toBreak < 0) {
      return false;
    }
    const Coordinates& newCoordinates = getHangingCoordinates(toBreak, gNodes);
    breakTriangle(toBreak, newCoordinates, gNodes, triangle);
    return true;
  }

private:
  const Coordinates& getHangingCoordinates(int brokenEdge,
                                           const vector<GNode>& gNodes) const {
    std::vector<GNode> edgeHalves;
    edgeHalves.reserve(2);
    auto outEdges = graph->getGraph()->out_edges(gNodes[brokenEdge]);
    std::transform(
        outEdges.begin(), outEdges.end(), std::back_inserter(edgeHalves),
        [&](auto& edge) { return graph->getGraph()->getEdgeDst(edge); });
    auto hangingCoordinates = Edge::getCommonPoint(edgeHalves[0]->getData(),
                                                   edgeHalves[1]->getData());
    return hangingCoordinates.get();
  }

  int chooseEdge(const Edge& triangle, const vector<GNode>& edges) {
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
