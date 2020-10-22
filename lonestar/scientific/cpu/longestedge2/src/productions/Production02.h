#ifndef GALOIS_PRODUCTION02_H
#define GALOIS_PRODUCTION02_H

#include <galois/UserContext.h>
#include "../model/Graph.h"
#include "../model/ProductionState.h"
#include "../model/Edge.h"
#include "../model/MyGraph.h"
#include "../utils/ProductionHelpers.h"
class Production02 {
public:
  bool execute(GNode& triangle, MyGraph& graph, Bag &bag,
               galois::UserContext<GNode>& ctx) {
    auto edges = graph.getEdges(triangle);
    auto toBreak = whichToBreak(triangle, edges);
    if (toBreak < 0) {
      return false;
    }
    fixTriangle(toBreak, edges, triangle, bag, ctx);
  }

private:
  int whichToBreak(const Edge& triangle, const vector<Edge>& edges) {
    const vector<int>& longest = getLongest(edges);
    vector<int> toBreak;
    for(auto edge: longest) {
      if (edges[edge].isBroken()) {
        toBreak.push_back(edge);
      }
    }
    return chooseGreatest(toBreak, edges);
  }
  void fixTriangle(int toBreak, vector<Edge> edges, GNode& triangle, Bag &bag,
                   galois::UserContext<GNode>& ctx) {}
};

#endif // GALOIS_PRODUCTION02_H
