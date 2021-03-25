#ifndef GALOIS_GRAPHGENERATOR_H
#define GALOIS_GRAPHGENERATOR_H

#include "Utils.h"
#include "../model/GraphAdapter.h"

class GraphGenerator {
public:
  static void
  prepare12TrianglesGraph(const double west_border, const double north_border,
                          const double east_border, const double south_border,
                          bool version2D, GraphAdapter* graph,
                          galois::InsertBag<Coordinates>* coordsBag, Map* map) {

    /*
     * 8-e20-9-e21-10e22-11
     * |    /|    /|    /|
     * e10e11e13e14e16e17e19
     * |/    |/    |/    |
     * 4-e12-5-e15-6-e18-7
     * |    /|    /|    /|
     * e0 e1 e3 e4 e6 e7 e9
     * |/    |/    |/    |
     * 0--e2-1--e5-2--e8-3
     */

    vector<reference_wrapper<const Coordinates>> coords = prepare12CoordsVector(
        west_border, north_border, east_border, south_border, coordsBag, map);
    std::vector<Edge> edges{};

    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 3; ++k) {
        edges.emplace_back(
            Edge{coords[j * 4 + k], coords[j * 4 + k + 4], k == 0, version2D});
        edges.emplace_back(
            Edge{coords[j * 4 + k], coords[j * 4 + k + 5], false, version2D});
        edges.emplace_back(
            Edge{coords[j * 4 + k], coords[j * 4 + k + 1], j == 0, version2D});
      }
      // east line
      edges.emplace_back(
          Edge{coords[j * 4 + 3], coords[j * 4 + 7], true, version2D});
    }
    // north line
    for (int i = 8; i < 11; ++i) {
      edges.emplace_back(Edge{coords[i], coords[(i + 1)], true, version2D});
    }

    std::vector<GNode> gNodes{};
    for (const Edge& edge : edges) {
      gNodes.emplace_back(graph->createAndAddNode(edge));
    }

    for (int i = 0; i < 2; ++i) {
      for (int l = 0; l < 3; ++l) {
        size_t north  = i == 0 ? l * 3 + 12 : 20 + l;
        size_t west   = i * 10 + l * 3;
        size_t south  = i * 10 + l * 3 + 2;
        size_t east   = i * 10 + l * 3 + 3;
        size_t middle = i * 10 + l * 3 + 1;
        gNodes.emplace_back(graph->createAndAddNode(Edge()));
        connectTriangle(gNodes.size() - 1, {south, east, middle}, gNodes, graph);
        gNodes.emplace_back(graph->createAndAddNode(Edge()));
        connectTriangle(gNodes.size() - 1, {north, west, middle}, gNodes, graph);
      }
    }
  }

  static void
  prepareSimplestGraph(const double west_border, const double north_border,
                       const double east_border, const double south_border,
                       bool version2D, GraphAdapter* graph,
                       galois::InsertBag<Coordinates>* coordsBag, Map* map) {

    vector<reference_wrapper<const Coordinates>> coords = prepareCoordsVector(
        west_border, north_border, east_border, south_border, coordsBag, map);
    std::vector<Edge> edges{};

    for (int i = 0; i < 4; ++i) {
      edges.emplace_back(Edge{coords[i], coords[(i + 1) % 4], true, version2D});
    }
    edges.emplace_back(Edge{coords[0], coords[2], false, version2D});

    std::vector<GNode> gNodes{};
    gNodes.emplace_back(graph->createAndAddNode(Edge()));
    gNodes.emplace_back(graph->createAndAddNode(Edge()));
    for (const Edge& edge : edges) {
      gNodes.emplace_back(graph->createAndAddNode(edge));
    }

    connectTriangle(0, {2, 3, 6}, gNodes, graph);
    connectTriangle(1, {4, 5, 6}, gNodes, graph);
  }

private:
  static vector<reference_wrapper<const Coordinates>>
  prepareCoordsVector(const double west_border, const double north_border,
                      const double east_border, const double south_border,
                      galois::InsertBag<Coordinates>* coordsBag, Map* map) {
    vector<reference_wrapper<const Coordinates>> coords;
    std::pair<double, double> sw;
    std::pair<double, double> se;
    std::pair<double, double> ne;
    std::pair<double, double> nw;

    if (map->isUtm()) {
      sw = Utils::convertToUtm(south_border, west_border, map);
      se = Utils::convertToUtm(south_border, east_border, map);
      ne = Utils::convertToUtm(north_border, east_border, map);
      nw = Utils::convertToUtm(north_border, west_border, map);
    } else {
      sw = make_pair(west_border, south_border);
      se = make_pair(east_border, south_border);
      ne = make_pair(east_border, north_border);
      nw = make_pair(west_border, north_border);
    }
    coords.emplace_back(
        cref(coordsBag->emplace_back(Coordinates{sw.first, sw.second, *map})));
    coords.emplace_back(
        cref(coordsBag->emplace_back(Coordinates{se.first, se.second, *map})));
    coords.emplace_back(
        cref(coordsBag->emplace_back(Coordinates{ne.first, ne.second, *map})));
    coords.emplace_back(
        cref(coordsBag->emplace_back(Coordinates{nw.first, nw.second, *map})));
    return coords;
  }

  static vector<reference_wrapper<const Coordinates>>
  prepare12CoordsVector(const double west_border, const double north_border,
                        const double east_border, const double south_border,
                        galois::InsertBag<Coordinates>* coordsBag, Map* map) {
    vector<reference_wrapper<const Coordinates>> coords;
    std::pair<double, double> sw;
    std::pair<double, double> se;
    std::pair<double, double> ne;
    std::pair<double, double> nw;

    if (map->isUtm()) {
      sw = Utils::convertToUtm(south_border, west_border, map);
      se = Utils::convertToUtm(south_border, east_border, map);
      ne = Utils::convertToUtm(north_border, east_border, map);
      nw = Utils::convertToUtm(north_border, west_border, map);
    } else {
      sw = make_pair(west_border, south_border);
      se = make_pair(east_border, south_border);
      ne = make_pair(east_border, north_border);
      nw = make_pair(west_border, north_border);
    }
    coords.emplace_back(
        cref(coordsBag->emplace_back(Coordinates{sw.first, sw.second, *map})));
    coords.emplace_back(cref(coordsBag->emplace_back(
        Coordinates{(2 * sw.first + se.first) / 3,
                    (2 * sw.second + se.second) / 3, *map})));
    coords.emplace_back(cref(coordsBag->emplace_back(
        Coordinates{(sw.first + 2 * se.first) / 3,
                    (sw.second + 2 * se.second) / 3, *map})));
    coords.emplace_back(
        cref(coordsBag->emplace_back(Coordinates{se.first, se.second, *map})));

    coords.emplace_back(cref(coordsBag->emplace_back(Coordinates{
        (sw.first + nw.first) / 2, (sw.second + nw.second) / 2, *map})));
    coords.emplace_back(cref(coordsBag->emplace_back(Coordinates{
        (2 * sw.first + se.first + 2 * nw.first + ne.first) / 6,
        (2 * sw.second + se.second + 2 * nw.second + ne.second) / 6, *map})));
    coords.emplace_back(cref(coordsBag->emplace_back(Coordinates{
        (sw.first + 2 * se.first + nw.first + 2 * ne.first) / 6,
        (sw.second + 2 * se.second + nw.second + 2 * ne.second) / 6, *map})));
    coords.emplace_back(cref(coordsBag->emplace_back(Coordinates{
        (se.first + ne.first) / 2, (se.second + ne.second) / 2, *map})));

    coords.emplace_back(
        cref(coordsBag->emplace_back(Coordinates{nw.first, nw.second, *map})));
    coords.emplace_back(cref(coordsBag->emplace_back(
        Coordinates{(2 * nw.first + ne.first) / 3,
                    (2 * nw.second + ne.second) / 3, *map})));
    coords.emplace_back(cref(coordsBag->emplace_back(
        Coordinates{(nw.first + 2 * ne.first) / 3,
                    (nw.second + 2 * ne.second) / 3, *map})));
    coords.emplace_back(
        cref(coordsBag->emplace_back(Coordinates{ne.first, ne.second, *map})));

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
