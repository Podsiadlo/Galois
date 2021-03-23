#ifndef GALOIS_PRODUCTION0X_H
#define GALOIS_PRODUCTION0X_H

//#include <galois/UserContext.h>
#include "../model/Edge.h"
#include "../model/GraphAdapter.h"
//#include "../utils/ProductionHelpers.h"
class Production0x {
public:
  explicit Production0x(GraphAdapter* graph, bool version2D, const Map& map)
      : graph(graph), version2D(version2D), map(map) {}

  virtual bool execute(const GNode& triangle, Bag* bag) = 0;

protected:
  GraphAdapter* graph;

  const Coordinates& breakEdge(int toBreak, Bag* bag,
                               const vector<reference_wrapper<Edge>>& edges,
                               const vector<GNode>& gNodes);

  void breakTriangle(int brokenEdgeIdx, const Coordinates& hangingPoint,
                     vector<reference_wrapper<Edge>> edges,
                     vector<GNode> gNodes, GNode triangle);

private:
  bool version2D;
  const Map& map;

  int firstAdjacent(int broken) const { return (broken + 1) % 3; }
  int secondAdjacent(int broken) const { return (broken + 2) % 3; }
};

#endif // GALOIS_PRODUCTION0X_H
