#ifndef GALOIS_DUMMYCONDITIONCHECKER_H
#define GALOIS_DUMMYCONDITIONCHECKER_H

#include "ConditionChecker.h"

//! This condition checker always sets a hyperedge node to be refined and
//! returns true
class DummyConditionChecker : ConditionChecker {
public:
  //! Sets refinement and returns true for hyperedge nodes
  bool execute(GNode& node) override {
    Edge& edge = node->getData();
    if (!edge.isTriangle()) {
      return false;
    }
    edge.setToRefine(true);
    return true;
  }
};

#endif // GALOIS_DUMMYCONDITIONCHECKER_H
