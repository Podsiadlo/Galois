#ifndef GALOIS_GRAPHADAPTER_H
#define GALOIS_GRAPHADAPTER_H

#include "Graph.h"

class GraphAdapter : private boost::noncopyable {
public:
  GraphAdapter() : graph(new Graph{}) {}
  GraphAdapter(Graph* graph) : graph(graph) {}
  virtual ~GraphAdapter() { delete graph; }

  std::vector<GNode> getGNodesFrom(GNode parent, bool triangles) const {
    std::vector<GNode> vertices;
    for(auto edge : graph->out_edges(parent)) {
      auto neighbour = graph->getEdgeDst(edge);
      if (neighbour->getData().isTriangle() == triangles) {
        vertices.emplace_back(neighbour);
      }
    }
    return vertices;
  }

  GNode createAndAddNode(const Edge& nodeData) {
    auto* node = graph->createNode(nodeData);
    graph->addNode(node);
    return node;
  }

  std::vector<std::reference_wrapper<Edge>> getEdges(const std::vector<GNode>& gNodes) const {
    std::vector<std::reference_wrapper<Edge>> edges;
    for (GNode gNode : gNodes) {
      edges.emplace_back(gNode->getData());
    }
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

private:
  Graph* graph;
};

#endif // GALOIS_GRAPHADAPTER_H
