#ifndef GALOIS_PRODUCTION_HELPERS_H
#define GALOIS_PRODUCTION_HELPERS_H

#include "../model/Graph.h"

std::vector<int> getLongest(const GNode* edges);

int chooseGreatest(std::vector<int> toBreak, const GNode* edges);

bool isAnyBroken(const GNode* edges);



#endif //GALOIS_PRODUCTION_HELPERS_H