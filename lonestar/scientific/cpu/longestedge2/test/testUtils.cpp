#ifndef GALOIS_TEST_UTILS
#define GALOIS_TEST_UTILS

#include "../src/model/GraphAdapter.h"

void checkTriangleCorrectness(GNode const& parent,
                              set<Coordinates>& triangleCoords,
                              vector<GNode>& childrenTriangles,
                              vector<GNode>& edges, const GraphAdapter& graph);

std::vector<GNode>
prepareTest1Graph(GraphAdapter* graph,
                  galois::InsertBag<Coordinates>* coordsBag) {
  std::vector<std::reference_wrapper<const Coordinates>> coords;

  coords.emplace_back(std::cref(coordsBag->emplace_back(Coordinates(0, 0, 0))));
  coords.emplace_back(std::cref(coordsBag->emplace_back(Coordinates(0, 1, 0))));
  coords.emplace_back(std::cref(coordsBag->emplace_back(Coordinates(1, 0, 0))));
  std::vector<Edge> edges{};

  edges.emplace_back(Edge{coords[0], coords[1], true, true});
  edges.emplace_back(Edge{coords[0], coords[2], true, true});
  edges.emplace_back(Edge{coords[1], coords[2], true, true});

  std::vector<GNode> gNodes{};
  gNodes.emplace_back(graph->createAndAddNode(Edge()));
  gNodes.emplace_back(graph->createAndAddNode(edges[0]));
  gNodes.emplace_back(graph->createAndAddNode(edges[1]));
  gNodes.emplace_back(graph->createAndAddNode(edges[2]));

  graph->addEdge(gNodes[0], gNodes[1]);
  graph->addEdge(gNodes[0], gNodes[2]);
  graph->addEdge(gNodes[0], gNodes[3]);

  return gNodes;
}

std::vector<GNode>
prepareTest2Graph(GraphAdapter* graph,
                  galois::InsertBag<Coordinates>* coordsBag) {
  std::vector<std::reference_wrapper<const Coordinates>> coords;

  coords.emplace_back(std::cref(coordsBag->emplace_back(Coordinates(0, 0, 0))));
  coords.emplace_back(std::cref(coordsBag->emplace_back(Coordinates(0, 1, 0))));
  coords.emplace_back(std::cref(coordsBag->emplace_back(Coordinates(1, 0, 0))));
  coords.emplace_back(
      std::cref(coordsBag->emplace_back(Coordinates(0.5, 0.5, 0))));
  std::vector<Edge> edges{};

  edges.emplace_back(Edge{coords[0], coords[1], true, true});
  edges.emplace_back(Edge{coords[0], coords[2], true, true});
  edges.emplace_back(Edge{coords[1], coords[2], true, true});
  edges.emplace_back(Edge{coords[1], coords[3], true, true});
  edges.emplace_back(Edge{coords[2], coords[3], true, true});

  edges[2].setBroken(true);

  std::vector<GNode> gNodes{};
  gNodes.emplace_back(graph->createAndAddNode(Edge()));
  gNodes.emplace_back(graph->createAndAddNode(edges[0]));
  gNodes.emplace_back(graph->createAndAddNode(edges[1]));
  gNodes.emplace_back(graph->createAndAddNode(edges[2]));
  gNodes.emplace_back(graph->createAndAddNode(edges[3]));
  gNodes.emplace_back(graph->createAndAddNode(edges[4]));

  graph->addEdge(gNodes[0], gNodes[1]);
  graph->addEdge(gNodes[0], gNodes[2]);
  graph->addEdge(gNodes[0], gNodes[3]);

  graph->addEdge(gNodes[3], gNodes[4]);
  graph->addEdge(gNodes[3], gNodes[5]);

  return gNodes;
}

int countTriangles(const GraphAdapter& graph) {
  int counter = 0;
  for (auto n : *(graph.getGraph())) {
    if (n->getData().isTriangle()) {
      ++counter;
    }
  }
  return counter;
}

int countUnbrokenEdges(const GraphAdapter& graph) {
  int counter = 0;
  for (auto n : *(graph.getGraph())) {
    auto edge = n->getData();
    if (!(edge.isTriangle()) && !(edge.isBroken())) {
      ++counter;
    }
  }
  return counter;
}

void checkPostProductionCorrectness(const GraphAdapter& graph,
                                    const GNode& parent) {
  vector<GNode> childrenTriangles{};
  vector<GNode> edges{};
  set<Coordinates> parentTriangleCoords{};
  checkTriangleCorrectness(parent, parentTriangleCoords, childrenTriangles,
                           edges, graph);

  SECTION("Checking children triangles correctness") {
    CHECK(childrenTriangles.size() == 2);

    set<Coordinates> child1TriangleCoords{};
    vector<GNode> child1ChildrenTriangles{};
    vector<GNode> child1Edges{};

    set<Coordinates> child2TriangleCoords{};
    vector<GNode> child2ChildrenTriangles{};
    vector<GNode> child2Edges{};

    checkTriangleCorrectness(childrenTriangles[0], child1TriangleCoords,
                             child1ChildrenTriangles, child1Edges, graph);
    checkTriangleCorrectness(childrenTriangles[1], child2TriangleCoords,
                             child2ChildrenTriangles, child2Edges, graph);

    SECTION("Checking if children's coordinates match parent's ones") {
      auto comparator = [](Coordinates a, Coordinates b) {
        if (a.getX() < b.getX()) {
          return true;
        }
        if (b.getX() < a.getX()) {
          return false;
        }
        if (a.getY() < b.getY()) {
          return true;
        }
        if (b.getY() < a.getY()) {
          return false;
        }
        return a.getZ() < b.getZ();
      };
      set<Coordinates, decltype(comparator)> childCoords(comparator);
      childCoords.merge(child1TriangleCoords);
      childCoords.merge(child2TriangleCoords);
      CHECK(childCoords.size() == 4);
      for (auto coord : parentTriangleCoords) {
        CHECK(childCoords.find(coord) != childCoords.end());
      }
    }
  }
}
void checkTriangleCorrectness(GNode const& parent,
                              set<Coordinates>& triangleCoords,
                              vector<GNode>& childrenTriangles,
                              vector<GNode>& edges, const GraphAdapter& graph) {
  for (auto edge : graph.getGraph()->out_edges(parent)) {
    auto node      = graph.getGraph()->getEdgeDst(edge);
    Edge& edgeData = node->getData();
    if (!(edgeData.isTriangle())) {
      edges.emplace_back(node);
      triangleCoords.insert(edgeData.getNodes().first.get());
      triangleCoords.insert(edgeData.getNodes().second.get());
    } else {
      childrenTriangles.emplace_back(node);
    }
  }
  std::string testSectionName = "Checking correctness of triangle: ";
  for (auto coord : triangleCoords) {
    testSectionName.append(static_cast<std::string>(coord) + ", ");
  }
  SECTION(testSectionName) {
    CHECK(edges.size() == 3);
    CHECK(triangleCoords.size() == 3);
  }
}

#endif