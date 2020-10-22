#ifndef GALOIS_PRODUCTION01_H
#define GALOIS_PRODUCTION01_H

#include <galois/UserContext.h>
#include "../model/Graph.h"
#include "../model/ProductionState.h"
#include "../model/Edge.h"
#include "../model/MyGraph.h"
#include "../utils/ProductionHelpers.h"

class Production01 {
public:
  bool execute(GNode& triangle, MyGraph& graph, Bag &bag, galois::UserContext<GNode>& ctx) {
    auto edges = graph.getEdges(triangle);

    auto toBreak = whichToBreak(triangle, edges);
    if (toBreak < 0) {
      return false;
    }
    breakEdge(edges[toBreak], bag);
    fixTriangle(toBreak, edges, triangle, bag, ctx);
  }

private:

  void fixTriangle(int toBreak, vector<Edge> edges, GNode& triangle, Bag &bag,
                   galois::UserContext<GNode>& ctx) {}

  void breakEdge(Edge& toBreak, Bag& bag) {}


  int whichToBreak(const Edge& triangle, const vector<Edge>& edges) {
    if (!triangle.isTriangle()) {
      return -1;
    }
    if (!triangle.isToRefine() && !isAnyBroken(edges)) {
      return -1;
    }
    const vector<int>& longest = getLongest(edges);
    vector<int> toBreak;
    for (auto edge: longest) {
      if (!edges[edge].isBroken()) {
        toBreak.push_back(edge);
      }
    }
    return chooseGreatest(toBreak, edges);
  }
};

#endif // GALOIS_PRODUCTION01_H
