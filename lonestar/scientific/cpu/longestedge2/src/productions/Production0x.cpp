#include "Production0x.h"
const Coordinates&
Production0x::breakEdge(int toBreak, Bag* bag,
                        const vector<reference_wrapper<Edge>>& edges,
                        const vector<GNode>& gNodes) {
  Edge& edgeToBreak         = edges[toBreak].get();
  bool border               = edgeToBreak.isBorder();
  const Coordinates& middle = bag->emplace_back(edgeToBreak.getMiddle(
      [this](double x, double y) { return map.get_height(x, y); }));
  Edge newEdge1(edgeToBreak.getNodes().first, cref(middle), border, version2D);
  Edge newEdge2(cref(middle), edgeToBreak.getNodes().second, border, version2D);
  GNode newGNode1 = graph->createAndAddNode(newEdge1);
  GNode newGNode2 = graph->createAndAddNode(newEdge2);
  graph->addEdge(gNodes[toBreak], newGNode1);
  graph->addEdge(gNodes[toBreak], newGNode2);

  edgeToBreak.setBroken(true);
  return middle;
}

void Production0x::breakTriangle(int brokenEdgeIdx,
                                 const Coordinates& hangingPoint,
                                 vector<reference_wrapper<Edge>> edges,
                                 vector<GNode> gNodes, GNode triangle) {
  auto oppositePoint =
      Edge::getCommonPoint(edges[firstAdjacent(brokenEdgeIdx)],
                           edges[secondAdjacent(brokenEdgeIdx)])
          .get();
  GNode newNode = graph->createAndAddNode(
      Edge(oppositePoint, cref(hangingPoint), false, version2D));

  const vector<GNode>& edgeHalves = graph->getGNodesFrom(gNodes[brokenEdgeIdx]);
  if (Edge::getCommonPoint(edgeHalves[0]->getData(),
                           edges[firstAdjacent(brokenEdgeIdx)])
          .is_initialized()) {

    graph->createTriangle(newNode, gNodes[firstAdjacent(brokenEdgeIdx)],
                          edgeHalves[0], triangle);
    graph->createTriangle(newNode, gNodes[secondAdjacent(brokenEdgeIdx)],
                          edgeHalves[1], triangle);
  } else {

    graph->createTriangle(newNode, gNodes[firstAdjacent(brokenEdgeIdx)],
                          edgeHalves[1], triangle);
    graph->createTriangle(newNode, gNodes[secondAdjacent(brokenEdgeIdx)],
                          edgeHalves[0], triangle);
  }

  triangle->getData().setBroken(true);
  triangle->getData().setToRefine(false);
}
