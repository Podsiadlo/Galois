#ifndef GALOIS_NODEDATA_H
#define GALOIS_NODEDATA_H

#include "Coordinates.h"

class NodeData {
private:
  bool hyperEdge;     //!< Indicates if node is a metanode to track triangles
  std::pair<Coordinates, Coordinates> points; //!< Coordinates of nodes
  bool toRefine;      //!< Indicates if node needs to be refined
  bool hanging;       //!< Indicates hanging node status

public:
  NodeData(bool isHyperEdge, const std::pair<Coordinates, Coordinates>& points, bool hanging)
      : hyperEdge(isHyperEdge), points(points), toRefine(false), hanging(hanging) {
//    setCoords(points);
  }

  NodeData(bool isHyperEdge, bool toRefine)
      : hyperEdge(isHyperEdge), points(), toRefine(toRefine), hanging(false) {}

  NodeData(bool isHyperEdge, bool toRefine, std::pair<Coordinates, Coordinates> points)
      : hyperEdge(isHyperEdge), points(points), toRefine(toRefine), hanging(false) {
//    setCoords(points);
  }

  // self-explanatory set/get functions

  std::pair<Coordinates, Coordinates> getPoints() const { return points; }

  Coordinates getCoords() const {return Coordinates();}

//  void setCoords(const std::pair<Coordinates, Coordinates>& coordinates) {
//    NodeData::points.setXYZ(coordinates.getX(), coordinates.getY(),
//                            coordinates.getZ());
//  }
//
//  void setCoords(const double x, const double y, const double z) {
//    NodeData::points.setXYZ(x, y, z);
//  }

  bool isToRefine() const { return toRefine; }

  void setToRefine(bool refine) { NodeData::toRefine = refine; }

  bool isHanging() const { return hanging; }

  void setHanging(bool hangingNode) { NodeData::hanging = hangingNode; }

  bool isHyperEdge() const { return hyperEdge; }

//  bool operator==(const NodeData& rhs) const {
//    return hyperEdge == rhs.hyperEdge && coords == rhs.coords &&
//           (hyperEdge ? toRefine == rhs.toRefine : hanging == rhs.hanging);
//  }
//
//  bool operator!=(const NodeData& rhs) const { return !(rhs == *this); }
//
//  bool operator<(const NodeData& rhs) const {
//    if (hyperEdge < rhs.hyperEdge)
//      return true;
//    if (rhs.hyperEdge < hyperEdge)
//      return false;
//    if (coords < rhs.coords)
//      return true;
//    if (rhs.coords < coords)
//      return false;
//    if (hyperEdge) {
//      return toRefine < rhs.toRefine;
//    } else {
//      return hanging < rhs.hanging;
//    }
//  }

//  bool operator>(const NodeData& rhs) const { return rhs < *this; }
//
//  bool operator<=(const NodeData& rhs) const { return !(rhs < *this); }
//
//  bool operator>=(const NodeData& rhs) const { return !(*this < rhs); }
};

#endif // GALOIS_NODEDATA_H
