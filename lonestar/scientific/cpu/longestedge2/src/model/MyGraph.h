#ifndef GALOIS_MYGRAPH_H
#define GALOIS_MYGRAPH_H

#include "Graph.h"

class MyGraph : public Graph {
public:
  std::vector<GNode> getNeighbours(GNode &triangle) {
    std::vector<GNode> vertices;
    for (Graph::edge_iterator ii = this->edge_begin(triangle),
             ee = this->edge_end(triangle);
         ii != ee; ++ii) {
      vertices.push_back(this->getEdgeDst(ii));
    }
    return vertices;
  }

  std::vector<Edge> getEdges(GNode &triangle) {

  }
};

#endif // GALOIS_MYGRAPH_H
