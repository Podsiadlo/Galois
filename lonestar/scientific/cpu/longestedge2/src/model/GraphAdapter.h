#ifndef GALOIS_GRAPHADAPTER_H
#define GALOIS_GRAPHADAPTER_H

#include "Graph.h"

class GraphAdapter : private boost::noncopyable {
public:
  GraphAdapter(Graph* graph) : graph(graph) {}

  std::vector<GNode> getGNodesFrom(GNode parent) const {
    std::vector<GNode> vertices;
    for (Graph::edge_iterator ii = graph->edge_begin(parent),
             ee = graph->edge_end(parent);
         ii != ee; ++ii) {
      vertices.emplace_back(graph->getEdgeDst(ii));
    }
    return vertices;
  }

  GNode createAndAddNode(const Edge& nodeData) const {
    auto *node = graph->createNode(nodeData);
    graph->addNode(node);
    return node;
  }

  std::vector<Edge> getEdges(const std::vector<GNode>& gNodes) const {
    std::vector<Edge> edges;
    for(GNode gNode : gNodes) {
      edges.emplace_back(gNode->getData());
    }
    return edges;
  }

  void addEdge(GNode src, GNode dst) {
    graph->addEdge(src, dst);
  };

private:
  Graph* graph;
};

#endif // GALOIS_GRAPHADAPTER_H
