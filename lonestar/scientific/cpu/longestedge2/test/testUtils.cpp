#ifndef GALOIS_TEST_UTILS
#define GALOIS_TEST_UTILS

//#include "../src/utils/ConnectivityManager.h"

#include "../src/model/GraphAdapter.h"

// std::vector<GNode> generateSampleGraph(Graph& graph) {
//  std::vector<GNode> nodes;
//  ConnectivityManager connManager{graph};
//  nodes.push_back(
//      connManager.createNode(NodeData{false, Coordinates{0, 0, 0}, false}));
//  nodes.push_back(
//      connManager.createNode(NodeData{false, Coordinates{0, 1, 0}, false}));
//  nodes.push_back(
//      connManager.createNode(NodeData{false, Coordinates{1, 0, 0}, false}));
//  nodes.push_back(
//      connManager.createNode(NodeData{false, Coordinates{1, 1, 0}, false}));
//
//  connManager.createEdge(nodes[0], nodes[1], true, Coordinates{0, 0.5, 0}, 1);
//  connManager.createEdge(nodes[1], nodes[3], true, Coordinates{0.5, 1, 0}, 1);
//  connManager.createEdge(nodes[2], nodes[3], true, Coordinates{1, 0.5, 0}, 1);
//  connManager.createEdge(nodes[0], nodes[2], true, Coordinates{0.5, 0, 0}, 1);
//  connManager.createEdge(nodes[3], nodes[0], false, Coordinates{0.5, 0.5, 0},
//                         sqrt(2));
//
//  nodes.push_back(connManager.createInterior(nodes[0], nodes[1], nodes[3]));
//  nodes.push_back(connManager.createInterior(nodes[0], nodes[3], nodes[2]));
//  return nodes;
//}
void checkTriangleCorrectness(GNode const& parent,
                              set<Coordinates>& triangleCoords,
                              vector<GNode>& childrenTriangles,
                              vector<GNode>& edges, const GraphAdapter& graph);

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
  //  std::vector<GNode> childrenNodes{};
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
      set<Coordinates> childCoords(child1TriangleCoords);
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