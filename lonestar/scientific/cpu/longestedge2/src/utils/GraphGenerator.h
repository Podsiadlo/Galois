#ifndef GALOIS_GRAPHGENERATOR_H
#define GALOIS_GRAPHGENERATOR_H

#include "Utils.h"
#include "../model/GraphAdapter.h"

class GraphGenerator {
public:
  static vector<GNode>
  prepareSimplestGraph(const double west_border, const double north_border,
                       const double east_border, const double south_border,
                       bool version2D, GraphAdapter* graph,
                       galois::InsertBag<Coordinates>* coordsBag,
                       Map* map) {

    vector<reference_wrapper<const Coordinates>> coords = prepareCoordsVector(
        west_border, north_border, east_border, south_border, coordsBag, map);
    std::vector<Edge> edges{};

    for (int i = 0; i < 4; ++i) {
      edges.emplace_back(Edge{coords[i], coords[(i+1)%4], true, version2D});
    }
    edges.emplace_back(Edge{coords[0], coords[2], false, version2D});

    std::vector<GNode> gNodes{};
    gNodes.emplace_back(graph->createAndAddNode(Edge()));
    gNodes.emplace_back(graph->createAndAddNode(Edge()));
    for (const Edge& edge : edges) {
      gNodes.emplace_back(graph->createAndAddNode(edge));
    }

    connectTriangle(0, std::vector<size_t>{2, 3, 6}, gNodes, graph);
    connectTriangle(1, std::vector<size_t>{4, 5, 6}, gNodes, graph);

    return gNodes;
  }

private:
  static vector<reference_wrapper<const Coordinates>>
  prepareCoordsVector(const double west_border, const double north_border,
                      const double east_border, const double south_border,
                      galois::InsertBag<Coordinates>* coordsBag, Map* map) {
    vector<reference_wrapper<const Coordinates>> coords;
    if (map->isUtm()) {
      coords.emplace_back(cref(coordsBag->emplace_back(Coordinates(
          Utils::convertToUtm(south_border, west_border, map), *map))));
      coords.emplace_back(cref(coordsBag->emplace_back(Coordinates{
          Utils::convertToUtm(south_border, east_border, map), *map})));
      coords.emplace_back(cref(coordsBag->emplace_back(Coordinates{
          Utils::convertToUtm(north_border, east_border, map), *map})));
      coords.emplace_back(cref(coordsBag->emplace_back(Coordinates{
          Utils::convertToUtm(north_border, west_border, map), *map})));
    } else {
      coords.emplace_back(cref(coordsBag->emplace_back(Coordinates(
          west_border, south_border, *map))));
      coords.emplace_back(cref(coordsBag->emplace_back(Coordinates{
          east_border, south_border, *map})));
      coords.emplace_back(cref(coordsBag->emplace_back(Coordinates{
          east_border, north_border, *map})));
      coords.emplace_back(cref(coordsBag->emplace_back(Coordinates{
          west_border, north_border, *map})));
    }
    return coords;
  }

  static void connectTriangle(size_t triangleNodeNo,
                              const std::vector<size_t>& edgeNodeNos,
                              const vector<GNode>& gNodes,
                              GraphAdapter* graph) {
    for (size_t edgeNo : edgeNodeNos) {
      graph->addEdge(gNodes[triangleNodeNo], gNodes[edgeNo]);
    }
  }
};

#endif // GALOIS_GRAPHGENERATOR_H
