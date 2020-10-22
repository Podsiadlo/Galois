#ifndef GALOIS_GRAPH_H
#define GALOIS_GRAPH_H

#include <galois/graphs/MorphGraph.h>
#include "NodeData.h"
#include "EdgeData.h"
#include "Edge.h"

using Graph        = galois::graphs::MorphGraph<Edge, void, false>;
using GNode        = Graph::GraphNode;
using EdgeIterator = Graph::edge_iterator;
using galois::optional;
using Bag          = galois::InsertBag<Coordinates>;

#endif // GALOIS_GRAPH_H
