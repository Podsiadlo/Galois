#include "AvsUcdWriter.h"

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


AvsUcdWriter::Triangle::Triangle(size_t firstEdge, size_t secondEdge,
                                 std::size_t thirdEdge, int materialId)
    : materialId(materialId) {
  edges.emplace_back(firstEdge);
  edges.emplace_back(secondEdge);
  edges.emplace_back(thirdEdge);
  std::sort(edges.begin(), edges.end());
}

bool AvsUcdWriter::Triangle::operator<(const AvsUcdWriter::Triangle& rhs) {
  if (edges[0] < rhs.edges[0]) {
    return true;
  }
  if (edges[0] == rhs.edges[0]) {
    if (edges[1] < rhs.edges[1]) {
      return true;
    }
    return edges[1] == rhs.edges[1] && edges[2] < rhs.edges[2];
  }
  return false;
}


void AvsUcdWriter::fillMaps() {
  size_t pointCounter     = 1;
  size_t segmentCounter   = 1;
  size_t trianglesCounter = 1;
  for (GNode node : *graph) {
    auto edge = node->getData();
    if (!edge.isTriangle()) {
      putOrGetIndex(convertToSegment(edge, &pointCounter), segments,
                    &segmentCounter);
    } else {
      putOrGetIndex(convertToTriangle(node, &pointCounter, &segmentCounter),
                    triangles, &trianglesCounter);
    }
  }
}

AvsUcdWriter::Segment AvsUcdWriter::convertToSegment(const Edge& edge,
                                                     size_t* pointCounter) {
  int materialId         = calculateSegmentMaterialId(edge);
  const Segment& segment = Segment{putOrGetIndex(edge.getNodes().first.get(), points, pointCounter),
                                   putOrGetIndex(putOrGetIndex(edge.getNodes().second.get(), points, pointCounter),
                                                 materialId};
  return segment;
}

int AvsUcdWriter::calculateSegmentMaterialId(const Edge& edge) const {
  int materialId = edge.isBorder() ? 1 : 3;
  materialId += edge.isBroken() ? 1 : 0;
  return materialId;
}

AvsUcdWriter::Triangle AvsUcdWriter::convertToTriangle(GNode node,
                                                       size_t* pointCounter,
                                                       size_t* segmentCounter) {
  const vector<GNode>& childNodes =
      GraphAdapter::getGNodesFrom(node, graph);
  std::vector<Segment> childSegments;
  for (auto* childNode : childNodes) {
    const Edge& childEdge = childNode->getData();
    if (!childEdge.isTriangle()) {
      childSegments.emplace_back(
          convertToSegment(childEdge, pointCounter));
    }
  }
  int materialId = 0;
  Triangle triangle{
      putOrGetIndex(childSegments[0], segments, segmentCounter),
      putOrGetIndex(childSegments[1], segments, segmentCounter),
      putOrGetIndex(childSegments[2], segments, segmentCounter), materialId};
  return triangle;
}

template <class T>
size_t AvsUcdWriter::putOrGetIndex(const T& element, map<T, size_t>& elements,
                                   size_t* counter) {
  auto foundIterator = elements.find(element);
  if (foundIterator == elements.end()) {
    elements.insert(make_pair(element, *counter));
    return (*counter)++;
  }
  return foundIterator->second;
}
