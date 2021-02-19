#include "../testUtils.cpp"
#include "../../src/productions/Production01.h"

std::vector<GNode>
prepareTest1Graph(GraphAdapter* graph,
                  galois::InsertBag<Coordinates>* coordsBag) {
  std::vector<std::reference_wrapper<const Coordinates>> coords;

  coords.emplace_back(std::cref(coordsBag->emplace_back(Coordinates(0, 0, 0))));
  coords.emplace_back(std::cref(coordsBag->emplace_back(Coordinates(0, 1, 0))));
  coords.emplace_back(std::cref(coordsBag->emplace_back(Coordinates(1, 0, 0))));
  //  coordsBag.emplace_back(Coordinates(1, 1, 0));
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

TEST_CASE("Production1 simple Test") {
  galois::SharedMemSys G;
  GraphAdapter graph{};
  galois::InsertBag<Coordinates> coordsBag;
  vector<GNode> nodes = prepareTest1Graph(&graph, &coordsBag);
  nodes[0]->getData().setToRefine(true);
  galois::UserContext<GNode> ctx;
  //  ConnectivityManager connManager{graph};
  Production01 production{&graph};
  //  ProductionState pState(connManager, nodes[5], false,
  //                         [](double, double) { return 0.; });
  production.execute(nodes[0], &coordsBag);

  CHECK(countTriangles(graph) == 3);
  CHECK(countUnbrokenEdges(graph) == 5);

  checkPostProductionCorrectness(graph, nodes[0]);
}

// TEST_CASE("Production1 complex Test") {
//  galois::SharedMemSys G;
//  Graph graph{};
//  vector<GNode> nodes = generateTest1Graph(graph);
//  nodes[6]->getData().setToRefine(true);
//  nodes[7]->getData().setToRefine(true);
//  nodes[8]->getData().setToRefine(true);
//  nodes[9]->getData().setToRefine(true);
//  galois::UserContext<GNode> ctx;
//  ConnectivityManager connManager{graph};
//  Production1 production{connManager};
//  ProductionState pState1(connManager, nodes[6], false,
//                          [](double, double) { return 0.; });
//  production.execute(pState1, ctx);
//  ProductionState pState2(connManager, nodes[7], false,
//                          [](double, double) { return 0.; });
//  production.execute(pState2, ctx);
//  ProductionState pState3(connManager, nodes[8], false,
//                          [](double, double) { return 0.; });
//  production.execute(pState3, ctx);
//  ProductionState pState4(connManager, nodes[9], false,
//                          [](double, double) { return 0.; });
//  production.execute(pState4, ctx);
//
//  REQUIRE(countTriangles(graph) == 6);
//  REQUIRE(countVertices(graph) == 8);
//}
