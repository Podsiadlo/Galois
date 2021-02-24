#ifndef GALOIS_PRODUCTION01_H
#define GALOIS_PRODUCTION01_H

#include <galois/UserContext.h>
#include "../model/Edge.h"
#include "../model/GraphAdapter.h"
#include "../utils/ProductionHelpers.h"

class Production01 {
public:
  Production01(GraphAdapter* graph, bool version2D)
      : graph(graph), version2D(version2D) {}

  bool execute(const GNode& triangle, Bag* bag) {
    auto gNodes = graph->getGNodesFrom(triangle, false);
    auto edges  = graph->getEdges(gNodes);

    int toBreak = chooseEdge(triangle->getData(), edges);
    if (toBreak < 0) {
      return false;
    }
    const Coordinates& newCoordinates = breakEdge(toBreak, bag, edges, gNodes);
    fixTriangle(toBreak, newCoordinates, edges, gNodes, triangle);
    return true;
  }

private:
  GraphAdapter* graph;
  bool version2D;

  void fixTriangle(int toBreak, const Coordinates& hangingPoint,
                   vector<std::reference_wrapper<Edge>> edges,
                   vector<GNode> gNodes, GNode triangle) {
    reference_wrapper<const Coordinates> oppositePoint =
        Edge::getCommonPoint(edges[firstOpposite(toBreak)],
                             edges[secondOpposite(toBreak)])
            .get();
    GNode newNode = graph->createAndAddNode(
        Edge(oppositePoint, std::cref(hangingPoint), false, version2D));
    GNode newTriangle1 = graph->createAndAddNode(Edge());
    GNode newTriangle2 = graph->createAndAddNode(Edge());
    graph->addEdge(triangle, newTriangle1);
    graph->addEdge(triangle, newTriangle2);

    graph->addEdge(newTriangle1, newNode);
    graph->addEdge(newTriangle2, newNode);

    graph->addEdge(newTriangle1, gNodes[firstOpposite(toBreak)]);
    graph->addEdge(newTriangle2, gNodes[secondOpposite(toBreak)]);

    const vector<GNode>& edgeHalves =
        graph->getGNodesFrom(gNodes[toBreak], false);
    if (Edge::getCommonPoint(edgeHalves[0]->getData(),
                             edges[firstOpposite(toBreak)])) {
      graph->addEdge(edgeHalves[0], gNodes[firstOpposite(toBreak)]);
      graph->addEdge(edgeHalves[1], gNodes[secondOpposite(toBreak)]);
    } else {
      graph->addEdge(edgeHalves[1], gNodes[firstOpposite(toBreak)]);
      graph->addEdge(edgeHalves[0], gNodes[secondOpposite(toBreak)]);
    }

    triangle->getData().setBroken(true);
    triangle->getData().setToRefine(false);
  }

  const Coordinates& breakEdge(int toBreak, Bag* bag,
                        const vector<std::reference_wrapper<Edge>>& edges,
                        const vector<GNode>& gNodes) {
    Edge& edgeToBreak   = edges[toBreak].get();
    bool border         = edgeToBreak.isBorder();
    const Coordinates& middle = bag->emplace_back(edgeToBreak.getMiddle());
    Edge newEdge1(edgeToBreak.getNodes().first, std::cref(middle), border,
                  version2D);
    Edge newEdge2(std::cref(middle), edgeToBreak.getNodes().second, border,
                  version2D);
    GNode newGNode1 = graph->createAndAddNode(newEdge1);
    GNode newGNode2 = graph->createAndAddNode(newEdge2);
    edgeToBreak.setBroken(true);
    graph->addEdge(gNodes[toBreak], newGNode1);
    graph->addEdge(gNodes[toBreak], newGNode2);
    return middle;
  }

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

  int firstOpposite(int broken) const { return (broken + 1) % 3; }

  int secondOpposite(int broken) const { return (broken + 2) % 3; }
};

#endif // GALOIS_PRODUCTION01_H
