#ifndef GALOIS_EDGE_H
#define GALOIS_EDGE_H

#include <vector>
#include "Coordinates.h"
class Edge {
private:
  bool triangle;

  bool toRefine;

  bool broken;
  std::pair<Coordinates, Coordinates> nodes;
  double length;

public:
  Edge(bool isTriangle) : triangle(isTriangle), broken(false) {}

  bool isTriangle() const { return triangle; }
  void setTriangle(bool isTriangle) { Edge::triangle = isTriangle; }
  bool isToRefine() const { return toRefine; }
  void setToRefine(bool refine) { Edge::toRefine = refine; }
  bool isBroken() const { return broken; }
  void setBroken(bool isBroken) { Edge::broken = isBroken; }
  double getLength() const { return length; }
  void setLength(double l) { Edge::length = l; }

  Coordinates getCoords() const { return Coordinates();}
};

#endif // GALOIS_EDGE_H
