#ifndef GALOIS_PRODUCTION_HELPERS_H
#define GALOIS_PRODUCTION_HELPERS_H

#include "../model/Graph.h"

std::vector<int> getLongest(const std::vector<GNode>& edges);

int chooseGreatest(std::vector<int> toBreak, const std::vector<GNode>& edges);

bool isAnyBroken(const std::vector<GNode>& edges);



#endif //GALOIS_PRODUCTION_HELPERS_H