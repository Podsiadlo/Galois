#include "../../src/productions/Production02.h"

TEST_CASE("Production2 should work on simple P2 graph", "[Production2]") {
  galois::SharedMemSys G;
  GraphAdapter graph{};
  galois::InsertBag<Coordinates> coordsBag;
  vector<GNode> nodes = prepareTest2Graph(&graph, &coordsBag);
  nodes[0]->getData().setToRefine(true);
  galois::UserContext<GNode> ctx;
  Production02 production{&graph, true};

  production.execute(nodes[0], &coordsBag);

  CHECK(countTriangles(graph) == 3);
  CHECK(countUnbrokenEdges(graph) == 5);
  checkPostProductionCorrectness(graph, nodes[0]);
}

TEST_CASE("Production2 should not work on simple P1 graph", "[Production2]") {
  galois::SharedMemSys G;
  GraphAdapter graph{};
  galois::InsertBag<Coordinates> coordsBag;
  vector<GNode> nodes = prepareTest2Graph(&graph, &coordsBag);
  nodes[0]->getData().setToRefine(true);
  galois::UserContext<GNode> ctx;
  Production01 production{&graph, true};
  bool result = production.execute(nodes[0], &coordsBag);
  REQUIRE(result == false);
}
