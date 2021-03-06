#include "../../src/model/GraphAdapter.h"

TEST_CASE("GraphAdapter::getGNodesFrom simple test") {
  galois::SharedMemSys G;
  Graph graph{};
  GraphAdapter graphAdapter{&graph};
  Edge mainEdge{};
  mainEdge.setBroken(false);
  auto *mainNode = graph.createNode(mainEdge);
  graph.addNode(mainNode);
  Edge parentEdge{};
  parentEdge.setBroken(false);
  auto *parentNode = graph.createNode(parentEdge);
  graph.addNode(parentNode);
  Edge childEdge1{};
  childEdge1.setBroken(true);
  auto *childNode1 = graph.createNode(childEdge1);
  graph.addNode(childNode1);
  Edge childEdge2{};
  childEdge2.setBroken(true);
  auto *childNode2 = graph.createNode(childEdge2);
  graph.addNode(childNode2);
  Edge freeEdge2{};
  freeEdge2.setBroken(false);
  auto *freeNode2 = graph.createNode(freeEdge2);
  graph.addNode(freeNode2);
  Edge freeEdge1{};
  freeEdge1.setBroken(false);
  auto *freeNode1 = graph.createNode(freeEdge1);
  graph.addNode(freeNode1);

  graph.addEdge(parentNode, mainNode);
  graph.addEdge(mainNode, childNode1);
  graph.addEdge(mainNode, childNode2);

  auto childrenNodes = graphAdapter.getGNodesFrom(mainNode);

  REQUIRE(childrenNodes.size() == 2);
  CHECK(childrenNodes[0]->getData().isBroken() == true);
  CHECK(childrenNodes[1]->getData().isBroken() == true);
}