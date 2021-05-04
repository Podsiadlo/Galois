#include <fstream>
#include "AvsUcdWriter.h"

void AvsUcdWriter::write(const string& filename) {
  StateMaps state;
  fillMaps(&state);
  writeToFile(state, filename);
}

void AvsUcdWriter::fillMaps(StateMaps* state) {
  for (GNode node : *graph) {
    auto edge = node->getData();
    if (!edge.isTriangle()) {
      putOrGetIndex(convertToSegment(edge, state), &(state->segments),
                    &(state->cellCounter));
    } else {
      putOrGetIndex(convertToTriangle(node, state), &(state->triangles),
                    &(state->cellCounter));
    }
  }
}

void AvsUcdWriter::writeToFile(const StateMaps& state, const string& filename) const {
  ofstream file;
  file.open(filename, ios::out);

  file << state.pointCounter << " " << state.cellCounter << " 0 0 0" << endl;

  for (const auto& pointEntry : state.points) {
    const auto & coordinates = pointEntry.first;
    file << pointEntry.second << " " << coordinates.getX() << " "
         << coordinates.getY() << " " << coordinates.getZ() << endl;
  }

  for (const auto& segmentEntry : state.segments) {
    const auto & segment = segmentEntry.first;
    file << segmentEntry.second << " " << segment.materialId << " line "
         << segment.pointIds.first << " " << segment.pointIds.second << endl;
  }

  for(const auto& triangleEntry : state.triangles) {
    const auto & triangle = triangleEntry.first;
    file << triangleEntry.second << " " << triangle.materialId << " tri";
    for(auto point : triangle.points) {
      file << " " << point;
    }
    file << endl;
  }

  file.close();
}

AvsUcdWriter::Segment AvsUcdWriter::convertToSegment(const Edge& edge,
                                                     StateMaps* state) {
  int materialId = calculateSegmentMaterialId(edge);
  const Segment& segment{putOrGetIndex(edge.getNodes().first.get(),
                                       &(state->points), &(state->pointCounter)),
                         putOrGetIndex(edge.getNodes().second.get(),
                                       &(state->points), &(state->pointCounter)),
                         materialId};
  return segment;
}

AvsUcdWriter::Triangle AvsUcdWriter::convertToTriangle(const GNode& node,
                                                       StateMaps* state) {
  const GNode* childNodes = GraphAdapter::getGNodesFrom(node, graph);
  std::set<Coordinates> coordsSet;
  std::vector<Segment> childSegments;
  for (int i = 0; i < 3; ++i) {
//  }
//  for (auto* childNode : childNodes) {
    const Edge& childEdge = childNodes[i]->getData();
    if (!childEdge.isTriangle()) {
      coordsSet.insert(childEdge.getNodes().first);
      coordsSet.insert(childEdge.getNodes().second);
    }
  }
  delete childNodes;

  std::vector<size_t> points;
  std::transform(coordsSet.begin(), coordsSet.end(), std::back_inserter(points),
                 [&](const Coordinates& coords) {
                   return putOrGetIndex(coords, &(state->points), &(state->pointCounter));
  });

  if (points.size() != 3) {
    ostringstream stream;
    stream << "Incorrect triangle. Vertices: ";
    for (size_t point : points) {
      stream << point << " ";
    }
    stream << endl;
    galois::gError(stream.str());
  }
  int materialId = calculateTriangleMaterialId(node->getData());
  return Triangle{points[0], points[1], points[2], materialId};
}

int AvsUcdWriter::calculateSegmentMaterialId(const Edge& edge) const {
  int materialId = edge.isBorder() ? 2 : 1;
  materialId += edge.isBroken() ? 2 : 0;
  return materialId;
}

int AvsUcdWriter::calculateTriangleMaterialId(const Edge& triangle) const {
  int materialId = triangle.isBroken() ? 3 : 1;
  return materialId;
}

template <class T>
size_t AvsUcdWriter::putOrGetIndex(const T& element, map<T, size_t>* elements,
                                   size_t* counter) {
  auto foundIterator = elements->find(element);
  if (foundIterator == elements->end()) {
    elements->insert(make_pair(element, *counter));
    return (*counter)++;
  }
  return foundIterator->second;
}

AvsUcdWriter::Segment::Segment(size_t firstPoint, size_t secondPoint,
                               int materialId)
    : materialId(materialId) {
  if (firstPoint < secondPoint) {
    pointIds = std::make_pair(firstPoint, secondPoint);
  } else {
    pointIds = std::make_pair(secondPoint, firstPoint);
  }
}

bool AvsUcdWriter::Segment::operator<(const AvsUcdWriter::Segment& rhs) const {
  if (pointIds.first < rhs.pointIds.first) {
    return true;
  }
  return pointIds.first == rhs.pointIds.first &&
         pointIds.second < rhs.pointIds.second;
}

AvsUcdWriter::Triangle::Triangle(size_t firstPoint, size_t secondPoint,
                                 std::size_t thirdPoint, int materialId)
    : materialId(materialId) {
  points.emplace_back(firstPoint);
  points.emplace_back(secondPoint);
  points.emplace_back(thirdPoint);
  std::sort(points.begin(), points.end());
}

bool AvsUcdWriter::Triangle::operator<(
    const AvsUcdWriter::Triangle& rhs) const {
  if (points[0] < rhs.points[0]) {
    return true;
  }
  if (points[0] == rhs.points[0]) {
    if (points[1] < rhs.points[1]) {
      return true;
    }
    return points[1] == rhs.points[1] && points[2] < rhs.points[2];
  }
  return false;
}
