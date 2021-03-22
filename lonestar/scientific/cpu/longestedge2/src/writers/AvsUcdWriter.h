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
    std::vector<size_t> points;
    int materialId;

    Triangle(size_t firstPoint, size_t secondPoint, std::size_t thirdPoint,
             int materialId);

    bool operator<(const Triangle& rhs) const;
  };

  struct StateMaps {
    std::map<Coordinates, size_t> points;
    std::map<Segment, size_t> segments;
    std::map<Triangle, size_t> triangles;
    size_t pointCounter{0};
    size_t cellCounter{0};
  };

  Graph* graph;

public:
  AvsUcdWriter(Graph* graph) : graph(graph) {}

  void write(const std::string& filename);

private:
  void fillMaps(StateMaps* state);

  void writeToFile(const StateMaps& state, const string& filename) const;

  AvsUcdWriter::Segment convertToSegment(const Edge& edge, StateMaps* state);

  AvsUcdWriter::Triangle convertToTriangle(const GNode& node, StateMaps* state);

  int calculateSegmentMaterialId(const Edge& edge) const;

  template <class T>
  size_t putOrGetIndex(const T& element, map<T, size_t>* elements,
                       size_t* counter);
};

#endif // GALOIS_AVSUCDWRITER_H
