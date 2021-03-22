#ifndef GALOIS_AVSUCDWRITER_H
#define GALOIS_AVSUCDWRITER_H

#include <string>
#include "../model/Graph.h"
#include "../model/GraphAdapter.h"
class AvsUcdWriter {
private:
  struct Segment {
    std::pair<size_t, size_t> pointIds;
    int materialId;

    Segment(size_t firstPoint, size_t secondPoint, int materialId);

    bool operator<(const Segment& rhs) const;
  };

  struct Triangle {
    std::vector<size_t> edges;
    int materialId;

    Triangle(size_t firstEdge, size_t secondEdge, std::size_t thirdEdge,
             int materialId);

    bool operator<(const Triangle& rhs) const;
  };

  Graph* graph;

  std::map<Coordinates, size_t> points;

  std::map<Segment, size_t> segments;

  std::map<Triangle, size_t> triangles;

public:
  AvsUcdWriter(Graph* graph) : graph(graph) {}

  void write(const std::string& filename) {
    fillMaps();
    writeToFile(filename);
  }

private:
  void fillMaps();

  void writeToFile(const string& filename);

  Segment convertToSegment(const Edge& edge, size_t* pointCounter);

  Triangle convertToTriangle(const GNode& node, size_t* pointCounter,
                             size_t* segmentCounter);

  int calculateSegmentMaterialId(const Edge& edge) const;

  template <class T>
  size_t putOrGetIndex(const T& element, std::map<T, size_t>& elements,
                       size_t* counter);
};

#endif // GALOIS_AVSUCDWRITER_H
