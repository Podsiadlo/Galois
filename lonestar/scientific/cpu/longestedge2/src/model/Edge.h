#ifndef GALOIS_EDGE_H
#define GALOIS_EDGE_H

#include <vector>
#include <functional>
#include <galois/optional.h>
#include "Coordinates.h"

using namespace std;

class Edge {
private:
  bool triangle{true};

  bool toRefine{false};

  bool broken{false};

  bool border{false};

  galois::optional<pair<reference_wrapper<const Coordinates>,
                        reference_wrapper<const Coordinates>>>
      nodes;

  double length{-1};

public:
  Edge()
      : nodes(galois::optional<pair<reference_wrapper<const Coordinates>,
                                    reference_wrapper<const Coordinates>>>()) {}

  Edge(reference_wrapper<const Coordinates> coordinates1,
       reference_wrapper<const Coordinates> coordinates2, bool border,
       bool version2D)
      : triangle(false), border(border),
        nodes(make_pair(cref(coordinates1), cref(coordinates2))),
        length(coordinates1.get().dist(coordinates2.get(), version2D)) {}

  Edge(const Coordinates& coordinates1, const Coordinates& coordinates2,
       bool border, bool version2D)
      : triangle(false), border(border),
        nodes(make_pair(cref(coordinates1), cref(coordinates2))),
        length(coordinates1.dist(coordinates2, version2D)) {}

  bool isTriangle() const { return triangle; }
  bool isToRefine() const { return toRefine; }
  void setToRefine(bool refine) { Edge::toRefine = refine; }
  bool isBorder() const { return border; }
  bool isBroken() const { return broken; }
  void setBroken(bool isBroken) { Edge::broken = isBroken; }
  double getLength() const { return length; }
  pair<reference_wrapper<const Coordinates>,
       reference_wrapper<const Coordinates>>
  getNodes() const {
    return nodes.get();
  }
  Coordinates getMiddle() const {
    return Coordinates(
        (nodes.get().first.get().getX() + nodes.get().second.get().getX()) / 2.,
        (nodes.get().first.get().getY() + nodes.get().second.get().getY()) /
            2.);
  }

  static galois::optional<reference_wrapper<const Coordinates>>
  getCommonPoint(const Edge& edge1, const Edge& edge2) {

    if (edge1.isTriangle() || edge2.isTriangle()) {
      fprintf(stderr, "Trying to get coords of triangle.");
      exit(19);
    }
    reference_wrapper<const Coordinates> coordinates1 = edge1.getNodes().first;
    reference_wrapper<const Coordinates> coordinates2 = edge1.getNodes().second;
    reference_wrapper<const Coordinates> coordinates3 = edge2.getNodes().first;
    reference_wrapper<const Coordinates> coordinates4 = edge2.getNodes().second;
    if (coordinates3.get() == coordinates1.get() ||
        coordinates3.get() == coordinates2.get()) {
      return galois::optional(coordinates3);
    }
    if (coordinates4.get() == coordinates1.get() ||
        coordinates4.get() == coordinates2.get()) {
      return galois::optional(coordinates4);
    }
    return galois::optional<reference_wrapper<const Coordinates>>();
  }
};

#endif // GALOIS_EDGE_H
