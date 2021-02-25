#ifndef GALOIS_PRODUCTION_HELPERS_H
#define GALOIS_PRODUCTION_HELPERS_H

#include "../model/Edge.h"

std::vector<int> getLongest(const std::vector<std::reference_wrapper<Edge>>& edges);

int chooseGreatest(std::vector<int> toBreak, const std::vector<std::reference_wrapper<Edge>>& edges);

bool isAnyBroken(const std::vector<std::reference_wrapper<Edge>> edges);



#endif //GALOIS_PRODUCTION_HELPERS_H