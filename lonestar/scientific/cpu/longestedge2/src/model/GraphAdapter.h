#ifndef GALOIS_GRAPHADAPTER_H
#define GALOIS_GRAPHADAPTER_H

#include "Graph.h"

class GraphAdapter : private boost::noncopyable {
public:
  GraphAdapter() : graph(new Graph{}), selfConstructedGraph(true) {}
  GraphAdapter(Graph* graph) : graph(graph) {}
  virtual ~GraphAdapter() {
    if (selfConstructedGraph) {
      delete graph;
    }
  }

  std::vector<GNode> getGNodesFrom(GNode parent) const {
    return getGNodesFrom(parent, graph);
  }

  std::vector<Coordinates>
  getCoordsOfTriangle(const GNode& triangleNode) const {
    std::set<Coordinates> coordsSet;
    const vector<GNode>& edgeNodes = getGNodesFrom(triangleNode);
    for (auto* node : edgeNodes) {
      auto edgeCoords = node->getData().getNodes();
      coordsSet.insert(edgeCoords.first);
      coordsSet.insert(edgeCoords.second);
    }
    return std::vector<Coordinates>{coordsSet.begin(), coordsSet.end()};
  }

  GNode createAndAddNode(const Edge& nodeData) {
    auto* node = graph->createNode(nodeData);
    graph->addNode(node);
    return node;
  }

  std::vector<std::reference_wrapper<Edge>>
  getEdges(const std::vector<GNode>& gNodes) const { // FIXME: Mem leak
    std::vector<std::reference_wrapper<Edge>> edges{
        gNodes[0]->getData(), gNodes[1]->getData(), gNodes[2]->getData()};
    //    edges.reserve(3);
    //    std::transform(gNodes.begin(), gNodes.end(),
    //    std::back_inserter(edges),
    //                   [](GNode node) { return (node->getData()); });
    return edges;
  }

  GNode createTriangle(const GNode& edge1, const GNode& edge2,
                       const GNode& edge3, const GNode& parent) {
    GNode newTriangle = createAndAddNode(Edge());
    graph->addEdge(parent, newTriangle);
    graph->addEdge(newTriangle, edge1);
    graph->addEdge(newTriangle, edge2);
    graph->addEdge(newTriangle, edge3);
    return newTriangle;
  }

  void addEdge(GNode src, GNode dst) { graph->addEdge(src, dst); };

  Graph* getGraph() const { return graph; }

  static std::vector<GNode> getGNodesFrom(GNode parent,
                                          Graph* graph) { // FIXME: Mem leak
    std::vector<GNode> vertices;
    //    vertices.reserve(3);
    auto outEdges = graph->out_edges(parent);
    std::transform(outEdges.begin(), outEdges.end(),
                   std::back_inserter(vertices),
                   [&graph](auto& edge) { return graph->getEdgeDst(edge); });
    return vertices;
  }

private:
  Graph* graph;
  bool selfConstructedGraph{false};
};

#endif // GALOIS_GRAPHADAPTER_H
