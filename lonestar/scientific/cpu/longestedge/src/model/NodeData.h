#ifndef GALOIS_NODEDATA_H
#define GALOIS_NODEDATA_H

#include "Coordinates.h"

#include <eigen3/Eigen/Dense>

typedef Eigen::Matrix<double, 16, 16> Matrix16d;
typedef Eigen::Matrix<double, 16, 1> Vector16d;

class NodeData {
private:
  bool hyperEdge;     //!< Indicates if node is a metanode to track triangles
  Coordinates coords; //!< Coordinates of node
  bool toRefine;      //!< Indicates if node needs to be refined
  bool hanging;       //!< Indicates hanging node status
  Matrix16d matrix;
  Vector16d result;

public:
  NodeData(bool isHyperEdge, const Coordinates& coords, bool hanging)
      : hyperEdge(isHyperEdge), coords(), toRefine(false), hanging(hanging),
        matrix(Matrix16d::Random()) {
    setCoords(coords);
  }

  NodeData(bool isHyperEdge, bool toRefine)
      : hyperEdge(isHyperEdge), coords(), toRefine(toRefine), hanging(false),
        matrix(Matrix16d::Random()) {}

  NodeData(bool isHyperEdge, bool toRefine, Coordinates coords)
      : hyperEdge(isHyperEdge), coords(), toRefine(toRefine), hanging(false),
        matrix(Matrix16d::Random()) {
    setCoords(coords);
  }

  // self-explanatory set/get functions

  Coordinates getCoords() const { return coords; }

  void setCoords(const Coordinates& coordinates) {
    NodeData::coords.setXYZ(coordinates.getX(), coordinates.getY(),
                            coordinates.getZ());
  }

  void setCoords(const double x, const double y, const double z) {
    NodeData::coords.setXYZ(x, y, z);
  }

  bool isToRefine() const { return toRefine; }

  void setToRefine(bool refine) { NodeData::toRefine = refine; }

  bool isHanging() const { return hanging; }

  void setHanging(bool hangingNode) { NodeData::hanging = hangingNode; }

  bool isHyperEdge() const { return hyperEdge; }

  const Matrix16d& getMatrix() const { return matrix; }

  void setResult(Vector16d result) { NodeData::result = result; }

  bool operator==(const NodeData& rhs) const {
    return hyperEdge == rhs.hyperEdge && coords == rhs.coords &&
           (hyperEdge ? toRefine == rhs.toRefine : hanging == rhs.hanging);
  }

  bool operator!=(const NodeData& rhs) const { return !(rhs == *this); }

  bool operator<(const NodeData& rhs) const {
    if (hyperEdge < rhs.hyperEdge)
      return true;
    if (rhs.hyperEdge < hyperEdge)
      return false;
    if (coords < rhs.coords)
      return true;
    if (rhs.coords < coords)
      return false;
    if (hyperEdge) {
      return toRefine < rhs.toRefine;
    } else {
      return hanging < rhs.hanging;
    }
  }

  bool operator>(const NodeData& rhs) const { return rhs < *this; }

  bool operator<=(const NodeData& rhs) const { return !(rhs < *this); }

  bool operator>=(const NodeData& rhs) const { return !(*this < rhs); }
};

#endif // GALOIS_NODEDATA_H
