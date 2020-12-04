#ifndef GALOIS_L2ERRORCOUNTER_H
#define GALOIS_L2ERRORCOUNTER_H

#include <cstdlib>
#include <cmath>
#include "../model/Coordinates.h"
class L2ErrorCounter {
private:
  double* errorTable;
  size_t width;
  size_t length;
  Map& map;
  double baseNorm;

  void fillErrorTableWithBaseFunction() {
    for (size_t i = 0; i < width; ++i) {
      for (size_t j = 0; j < length; ++j) {
        writeError(0, map.getData()[i][j], i, j);
      }
    }
  }

  void writeErrorValue(const double error, const size_t x, const size_t y) {
    errorTable[x * width + y] = error; // TODO: Check if x, y are not messed up
  }

  void
  compute_barycentric_coords(double* barycentric_coords, Coordinates& point,
                             const std::vector<Coordinates>& verticesCoords) {
    double triangle_area =
        get_area(verticesCoords[0], verticesCoords[1], verticesCoords[2]);
    barycentric_coords[2] =
        get_area(point, verticesCoords[0], verticesCoords[1]) / triangle_area;
    barycentric_coords[1] =
        get_area(point, verticesCoords[2], verticesCoords[0]) / triangle_area;
    barycentric_coords[0] =
        get_area(point, verticesCoords[1], verticesCoords[2]) / triangle_area;
  }

  bool is_inside_triangle(double barycentric_coords[]) {
    return !greater(barycentric_coords[0] + barycentric_coords[1] +
                        barycentric_coords[2],
                    1.);
  }

  double get_area(const Coordinates& a, const Coordinates& b,
                  const Coordinates& c) {
    return 0.5 * fabs((b.getX() - a.getX()) * (c.getY() - a.getY()) -
                      (b.getY() - a.getY()) * (c.getX() - a.getX()));
  }

  double getError() {
    double error = 0;
    for (size_t i = 0; i < width - 1; ++i) {
      for (size_t j = 0; j < length - 1; ++j) {
        error += readError(i, j) + readError(i, j + 1) +
                 readError(i + 1, j + 1) + readError(i + 1, j);
      }
    }
    return error * 0.25 * map.getCellWidth() * map.getCellLength();
  }

  void writeError(const double realVal, const double expectedVal,
                  const size_t x, const size_t y) {
    writeErrorValue(pow(realVal - expectedVal, 2), x, y);
  }

  double readError(const size_t x, const size_t y) const {
    return errorTable[x * width + y];
  }

  void computeBaseFunctionNorm() {
    fillErrorTableWithBaseFunction();
    baseNorm = popError();
  }

  void clear() {
    for (size_t i = 0; i < width * length; ++i) {
      errorTable[i] = 0;
    }
  }

public:
  L2ErrorCounter(Map& map)
      : width(map.getWidth()), length(map.getLength()), map(map) {
    errorTable = new double[width * length];
    computeBaseFunctionNorm();
  }

  void writeErrorOfTriangle(const std::vector<Coordinates>& verticesCoords) {

    // lowest x among 3
    double lowest_x = verticesCoords[0].getX() < verticesCoords[1].getX()
                          ? verticesCoords[0].getX()
                          : verticesCoords[1].getX();
    lowest_x = verticesCoords[2].getX() < lowest_x ? verticesCoords[2].getX()
                                                   : lowest_x;

    // highest x among 3
    double highest_x = verticesCoords[0].getX() > verticesCoords[1].getX()
                           ? verticesCoords[0].getX()
                           : verticesCoords[1].getX();
    highest_x = verticesCoords[2].getX() > highest_x ? verticesCoords[2].getX()
                                                     : highest_x;

    // lowest y among 3
    double lowest_y = verticesCoords[0].getY() < verticesCoords[1].getY()
                          ? verticesCoords[0].getY()
                          : verticesCoords[1].getY();
    lowest_y = verticesCoords[2].getY() < lowest_y ? verticesCoords[2].getY()
                                                   : lowest_y;

    // highest y among 3
    double highest_y = verticesCoords[0].getY() > verticesCoords[1].getY()
                           ? verticesCoords[0].getY()
                           : verticesCoords[1].getY();
    highest_y = verticesCoords[2].getY() > highest_y ? verticesCoords[2].getY()
                                                     : highest_y;

    double step = map.isUtm() ? 90 : map.getCellWidth();
    for (double i = lowest_x; i <= highest_x; i += step) {
      for (double j = lowest_y; j <= highest_y; j += step) {
        Coordinates tmp{i, j, 0.};
        double barycentric_point[3];
        compute_barycentric_coords(barycentric_point, tmp, verticesCoords);
        if (is_inside_triangle(barycentric_point)) {
          double point_height = 0;
          for (int k = 0; k < 3; ++k) {
            point_height += barycentric_point[k] * verticesCoords[k].getZ();
          }
          writeError(point_height, map.get_height(i, j), i / map.getCellWidth(),
                     j / map.getCellLength()); // TODO: Make it work with UTM!
        }
      }
    }
  }

  double popError() {
    double error = getError();
    clear();
    return error;
  }

  double popErrorAsRelative() {
    return popError() * 100 / baseNorm;
  }

  virtual ~L2ErrorCounter() { delete errorTable; }
};

#endif // GALOIS_L2ERRORCOUNTER_H
